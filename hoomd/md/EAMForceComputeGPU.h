// Copyright (c) 2009-2016 The Regents of the University of Michigan
// This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.



// Maintainer: morozov

/**
powered by:
Moscow group.
*/

#include "EAMForceCompute.h"
#include "NeighborList.h"
#include "EAMForceGPU.cuh"
#include "hoomd/Autotuner.h"

#include <boost/shared_ptr.hpp>

/*! \file EAMForceComputeGPU.h
    \brief Declares the class EAMForceComputeGPU
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

#ifndef __EAMForceComputeGPU_H__
#define __EAMForceComputeGPU_H__

//! Computes Lennard-Jones forces on each particle using the GPU
/*! Calculates the same forces as EAMForceCompute, but on the GPU by using texture memory(cudaArray) with hardware interpolation.
*/
class EAMForceComputeGPU : public EAMForceCompute
    {
    public:
        //! Constructs the compute
        EAMForceComputeGPU(boost::shared_ptr<SystemDefinition> sysdef, char *filename, int type_of_file);

        //! Destructor
        virtual ~EAMForceComputeGPU();

        //! Set autotuner parameters
        /*! \param enable Enable/disable autotuning
            \param period period (approximate) in time steps when returning occurs
        */
        virtual void setAutotunerParams(bool enable, unsigned int period)
            {
            EAMForceCompute::setAutotunerParams(enable, period);
            m_tuner->setPeriod(period);
            m_tuner->setEnabled(enable);
            }

    protected:
        EAMTexInterData eam_data;                   //!< Undocumented parameter
        EAMtex eam_tex_data;                        //!< Undocumented parameter
        Scalar * d_atomDerivativeEmbeddingFunction; //!< array F'(rho) for each particle
        boost::scoped_ptr<Autotuner> m_tuner;       //!< Autotuner for block size

        //! Actually compute the forces
        virtual void computeForces(unsigned int timestep);
    };

//! Exports the EAMForceComputeGPU class to python
void export_EAMForceComputeGPU();

#endif
