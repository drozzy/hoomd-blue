// Copyright (c) 2009-2016 The Regents of the University of Michigan
// This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.


// Maintainer: jglaser

#include "hoomd/ForceConstraint.h"
#include "NeighborList.h"

/*! \file MolecularForceCompute.h
    \brief Base class for ForceConstraints that depend on a molecular topology

    Implements the data structures that define a molecule topology.
    MolecularForceCompute maintains a list of local molecules and their constituent particles, and
    the particles are sorted according to global particle tag.

    The data structures are initialized by calling initMolecules(). This is done in the derived class
    whenever particles are reordered.

    Every molecule has a unique contiguous tag, 0 <=tag <m_n_molecules_global.

    Derived classes take care of resizing the ghost layer accordingly so that
    spanning molecules are communicated correctly. They connect to the Communciator
    signal using addGhostLayerWidthRequest() .

    In MPI simulations, MolecularForceCompute ensures that local molecules are complete by requesting communication of all
    members of a molecule even if only a single particle member falls into the ghost layer.
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

#ifndef __MolecularForceCompute_H__
#define __MolecularForceCompute_H__

const unsigned int NO_MOLECULE = (unsigned int)0xffffffff;

class MolecularForceCompute : public ForceConstraint
    {
    public:
        //! Constructs the compute
        MolecularForceCompute(boost::shared_ptr<SystemDefinition> sysdef);

        //! Destructor
        virtual ~MolecularForceCompute();

        //! Return the number of DOF removed by this constraint
        virtual unsigned int getNDOFRemoved() { return 0; }

        #ifdef ENABLE_MPI
        //! Get ghost particle fields requested by this pair potential
        virtual CommFlags getRequestedCommFlags(unsigned int timestep)
            {
            CommFlags flags = CommFlags(0);

            // request communication of tags
            flags[comm_flag::tag] = 1;

            flags |= ForceConstraint::getRequestedCommFlags(timestep);

            return flags;
            }
        #endif

        //! Return molecule index
        const Index2D& getMoleculeIndexer()
            {
            checkParticlesSorted();

            return m_molecule_indexer;
            }

        //! Return molecule list
        const GPUVector<unsigned int>& getMoleculeList()
            {
            checkParticlesSorted();

            return m_molecule_list;
            }

        //! Return molecule lengths
        const GPUVector<unsigned int>& getMoleculeLengths()
            {
            checkParticlesSorted();

            return m_molecule_length;
            }

        //! Return molecule order
        const GPUVector<unsigned int>& getMoleculeOrder()
            {
            checkParticlesSorted();

            return m_molecule_order;
            }

    protected:
        GPUVector<unsigned int> m_molecule_tag;     //!< Molecule tag per particle tag
        unsigned int m_n_molecules_global;          //!< Global number of molecules

    private:
        GPUVector<unsigned int> m_molecule_list;    //!< 2D Array of molecule members
        GPUVector<unsigned int> m_molecule_length;  //!< List of molecule lengths
        GPUVector<unsigned int> m_molecule_order;   //!< Order in molecule by local ptl idx

        Index2D m_molecule_indexer;                 //!< Index of the molecule table

        //! construct a list of local molecules
        virtual void initMolecules();

        //! Helper function to check if particles have been sorted and rebuild indices if necessary
        void checkParticlesSorted()
            {
            if (m_particles_sorted)
                {
                // rebuild molecule list
                initMolecules();
                m_particles_sorted = false;
                }
            }


    };

//! Exports the MolecularForceCompute to python
void export_MolecularForceCompute();

#endif
