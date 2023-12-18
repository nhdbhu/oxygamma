#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4UnitsTable.hh"

#include <iomanip>


class CrystalHit : public G4VHit
{
public:
    CrystalHit();
    CrystalHit(const CrystalHit&) = default;
    ~CrystalHit() override;

    CrystalHit& operator = (const CrystalHit&) = default;
    G4bool operator == (const CrystalHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void SetTime   (G4double time)   { fTime   = time;   };
    void SetEnergy (G4double energy) { fEnergy = energy; };
    void SetEdep   (G4double edep)   { fEdep   = edep;   };
    void SetPDG    (G4int pdg)       { fPDG    = pdg;    };
    void SetDet    (G4int det)       { fDet    = det;    };

    void AddEdep   (G4double edep)   { fEdep   += edep;  };

    G4double GetTime()   const { return fTime;   };
    G4double GetEnergy() const { return fEnergy; };
    G4double GetEdep()   const { return fEdep;   };
    G4int GetPDG()       const { return fPDG;    };
    G4int GetDet()       const { return fDet;    };

private:
    G4double fTime;
    G4double fEnergy;
    G4double fEdep;
    G4int fPDG;
    G4int fDet;
};


using HitsCollection = G4THitsCollection<CrystalHit>;
extern G4ThreadLocal G4Allocator<CrystalHit>* CrystalHitAllocator;


inline void* CrystalHit::operator new(size_t)
{
    if(!CrystalHitAllocator) CrystalHitAllocator = new G4Allocator<CrystalHit>;
    return (void *) CrystalHitAllocator->MallocSingle();
}


inline void CrystalHit::operator delete(void *hit)
{
    CrystalHitAllocator->FreeSingle((CrystalHit*) hit);
}

