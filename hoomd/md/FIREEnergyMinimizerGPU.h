// Copyright (c) 2009-2016 The Regents of the University of Michigan
// This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.


// Maintainer: askeys

#include "FIREEnergyMinimizer.h"

#include <boost/shared_ptr.hpp>

#ifndef __FIRE_ENERGY_MINIMIZER_GPU_H__
#define __FIRE_ENERGY_MINIMIZER_GPU_H__

/*! \file FIREEnergyMinimizer.h
    \brief Declares a base class for all energy minimization methods
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

//! Finds the nearest basin in the potential energy landscape
/*! \b Overview

    \ingroup updaters
*/
class FIREEnergyMinimizerGPU : public FIREEnergyMinimizer
    {
    public:
        //! Constructs the minimizer and associates it with the system
        FIREEnergyMinimizerGPU(boost::shared_ptr<SystemDefinition>, boost::shared_ptr<ParticleGroup>, Scalar);

        //! Destroys the minimizer
        virtual ~FIREEnergyMinimizerGPU() {}

        //! Resets the minimizer
        virtual void reset();

        //! Iterates forward one step
        virtual void update(unsigned int);

    protected:
        //! Creates the underlying NVE integrator
        virtual void createIntegrator();
        unsigned int m_nparticles;              //!< number of particles in the system
        unsigned int m_block_size;              //!< block size for partial sum memory
        unsigned int m_num_blocks;              //!< number of memory blocks reserved for partial sum memory
        GPUArray<Scalar> m_partial_sum1;         //!< memory space for partial sum over P
        GPUArray<Scalar> m_partial_sum2;         //!< memory space for partial sum over vsq
        GPUArray<Scalar> m_partial_sum3;         //!< memory space for partial sum over asq
        GPUArray<Scalar> m_sum;                  //!< memory space for sum over vsq
        GPUArray<Scalar> m_sum3;                 //!< memory space for the sum over P, vsq, asq

    private:

    };

//! Exports the FIREEnergyMinimizerGPU class to python
void export_FIREEnergyMinimizerGPU();

#endif // #ifndef __FIRE_ENERGY_MINIMIZER_GPU_H__
