#ifndef __TPM_IO_LIB_H__
#define __TPM_IO_LIB_H__

UINT64
EFIAPI
TpmIoRead64 (
  IN      UINT64                     Address
  );

UINT64
EFIAPI
TpmIoWrite64 (
  IN      UINTN                     Address,
  IN      UINT64                    Value
  );


UINT32
EFIAPI
TpmIoRead32 (
  IN      UINTN                     Address
  );

UINT32
EFIAPI
TpmIoWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  );


UINT16
EFIAPI
TpmIoRead16 (
  IN      UINTN                     Address
  );

UINT16
EFIAPI
TpmIoWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                    Value
  );


UINT8
EFIAPI
TpmIoRead8 (
  IN      UINTN                     Address
  );

UINT8
EFIAPI
TpmIoWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  );

#endif // __TPM_IO_LIB_H__