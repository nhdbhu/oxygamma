#include "CrystalHit.hh"


G4ThreadLocal G4Allocator<CrystalHit>* CrystalHitAllocator = nullptr;


CrystalHit::CrystalHit()
{}


CrystalHit::~CrystalHit()
{}


G4bool CrystalHit::operator == (const CrystalHit& right) const
{
    return ( this == &right ) ? true : false;
}

