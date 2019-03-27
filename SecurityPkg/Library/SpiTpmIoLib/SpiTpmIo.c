#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>


#include <Protocol/Spi.h>
#include <Protocol/SpiFlash.h>

BCM283X_SPI_MASTER_PROTOCOL *SpiMasterProtocol;

#define TRANSFER_HDR_LENGTH     4
#define TPM_RETRY               50
#define MAX_SPI_FRAMESIZE       64

STATIC SPI_DEVICE   *mSpiDevice;

STATIC
VOID
TpmIoSpiTransfer(UINTN Address, UINT16 Length, UINT8 * In, UINT8 * Out)
{
    UINT32 Retry;
    UINT16 BytesTransferred, TransferLength;
    UINT8 Header[TRANSFER_HDR_LENGTH];
    UINT8 Status[TRANSFER_HDR_LENGTH];
    UINT8 Ready, Poll;

    BytesTransferred = 0;

    while(BytesTransferred < Length) {
        TransferLength = ((Length - BytesTransferred) < 0x40) ? (Length - BytesTransferred) : 0x40;

        Header[0] = ((In != NULL) ? 0x80 : 0x00) | (TransferLength - 1);
        Header[1] = 0xD4;
        Header[2] = Address >> 8;
        Header[3] = Address;

        SpiMasterProtocol->Transfer(SpiMasterProtocol, mSpiDevice, 4, Header, Status, SPI_TRANSFER_BEGIN);

        Retry = 0;
        Poll = 0;
        Ready = Status[3];
        while(((Ready & 0x01) == 0) && (Retry++  < TPM_RETRY)){
            SpiMasterProtocol->Transfer(SpiMasterProtocol, mSpiDevice, 1, &Poll, &Ready, SPI_TRANSFER_CONTINUE);
        }

        SpiMasterProtocol->Transfer(SpiMasterProtocol, mSpiDevice, TransferLength, Out, In, SPI_TRANSFER_END);

        BytesTransferred += TransferLength;
    }
}

STATIC
VOID
TpmIoSpiReadBytes(UINTN Address, UINT16 Length, UINT8 * In)
{
    TpmIoSpiTransfer(Address, Length, In, NULL);
}

STATIC
VOID
TpmIoSpiWriteBytes(UINTN Address, UINT16 Length, UINT8 * Out)
{
    TpmIoSpiTransfer(Address, Length, NULL, Out);
}


UINT64
EFIAPI
TpmIoRead64 (
  IN      UINTN                     Address
  )
{
    /* Need cpu_to_le() */
    UINT64 Result;
    TpmIoSpiReadBytes(Address, 8, (UINT8*)&Result);
    return Result;
}

UINT64
EFIAPI
TpmIoWrite64 (
  IN      UINTN                     Address,
  IN      UINT64                    Value
  )
{
    TpmIoSpiWriteBytes(Address, 8, (UINT8*)&Value);
    return Value;
}



UINT32
EFIAPI
TpmIoRead32 (
  IN      UINTN                     Address
  )
{
    /* Need cpu_to_le() */
    UINT32 Result;
    TpmIoSpiReadBytes(Address, 4, (UINT8*)&Result);
    return Result;
}

UINT32
EFIAPI
TpmIoWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  )
{
    TpmIoSpiWriteBytes(Address, 4, (UINT8*)&Value);
    return Value;
}


UINT16
EFIAPI
TpmIoRead16 (
  IN      UINTN                     Address
  )
{
    UINT16 Result;
    TpmIoSpiReadBytes(Address, 2, (UINT8*)&Result);
    return Result;
}

UINT16
EFIAPI
TpmIoWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                     Value
  )
{
    TpmIoSpiWriteBytes(Address, 1, (UINT8*)&Value);
    return Value;
}


UINT8
EFIAPI
TpmIoRead8 (
  IN      UINTN                     Address
  )
{
    UINT8 Result;
    TpmIoSpiReadBytes(Address, 1, &Result);
    return Result;
}

UINT8
EFIAPI
TpmIoWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  )
{
    TpmIoSpiWriteBytes(Address, 1, &Value);
    return Value;
}





EFI_STATUS
EFIAPI
TpmIoSpiConstructor (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  mSpiDevice = AllocateRuntimeZeroPool (sizeof (SPI_DEVICE));
  if (mSpiDevice == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  mSpiDevice->Cs = 1;
  mSpiDevice->Controller = 0;
  mSpiDevice->MaxFreq = 100000;
  mSpiDevice->Mode = 0;

  Status = gBS->LocateProtocol (
    &gBcm283xSpiMasterProtocolGuid,
    NULL,
    (VOID **)&SpiMasterProtocol
  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "SpiFlash: Cannot locate SPI Master protocol\n"));
    return EFI_DEVICE_ERROR;
  }

  SpiMasterProtocol->SetupDevice(SpiMasterProtocol, 
                                  mSpiDevice,
                                  0,0,0,0);

  return Status;
}


