
#include <Library/IoLib.h>


UINT64
EFIAPI
TpmIoRead64 (
  IN      UINTN                     Address
  )
{
    return MmioRead64(Address);
}

UINT64
EFIAPI
TpmIoWrite64 (
  IN      UINTN                     Address,
  IN      UINT64                    Value
  )
{
    return MmioWrite64(Address, Value);    
}


UINT32
EFIAPI
TpmIoRead32 (
  IN      UINTN                     Address
  )
{
    return MmioRead32(Address);
}

UINT32
EFIAPI
TpmIoWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  )
{
    return MmioWrite32(Address, Value);    
}



UINT16
EFIAPI
TpmIoRead16 (
  IN      UINTN                     Address
  )
{
    return MmioRead16(Address);
}

UINT16
EFIAPI
TpmIoWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                     Value
  )
{
    return MmioWrite16(Address, Value);    
}


UINT8
EFIAPI
TpmIoRead8 (
  IN      UINTN                     Address
  )
{
    return MmioRead8(Address);
}

UINT8
EFIAPI
TpmIoWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  )
{
    return MmioWrite8(Address, Value);    
}


