// Copyright (c) 2009-2016 The Regents of the University of Michigan
// This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.


// Maintainer: dnlebard
#include "HarmonicImproperForceCompute.h"
#include "HarmonicImproperForceGPU.cuh"
#include "hoomd/Autotuner.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

/*! \file HarmonicImproperForceComputeGPU.h
    \brief Declares the HarmonicImproperForceGPU class
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

#ifndef __HARMONICIMPROPERFORCECOMPUTEGPU_H__
#define __HARMONICIMPROPERFORCECOMPUTEGPU_H__

//! Implements the harmonic improper force calculation on the GPU
/*! HarmonicImproperForceComputeGPU implements the same calculations as HarmonicImproperForceCompute,
    but executing on the GPU.

    Per-type parameters are stored in a simple global memory area pointed to by
    \a m_gpu_params. They are stored as Scalar2's with the \a x component being K and the
    \a y component being t_0.

    The GPU kernel can be found in improperforce_kernel.cu.

    \ingroup computes
*/
class HarmonicImproperForceComputeGPU : public HarmonicImproperForceCompute
    {
    public:
        //! Constructs the compute
        HarmonicImproperForceComputeGPU(boost::shared_ptr<SystemDefinition> sysdef);
        //! Destructor
        ~HarmonicImproperForceComputeGPU();

        //! Set autotuner parameters
        /*! \param enable Enable/disable autotuning
            \param period period (approximate) in time steps when returning occurs
        */
        virtual void setAutotunerParams(bool enable, unsigned int period)
            {
            HarmonicImproperForceCompute::setAutotunerParams(enable, period);
            m_tuner->setPeriod(period);
            m_tuner->setEnabled(enable);
            }

        //! Set the parameters
        virtual void setParams(unsigned int type, Scalar K, Scalar chi);

    protected:
        boost::scoped_ptr<Autotuner> m_tuner; //!< Autotuner for block size
        GPUArray<Scalar2>  m_params;          //!< Parameters stored on the GPU (k,chi)

        //! Actually compute the forces
        virtual void computeForces(unsigned int timestep);
    };

//! Export the ImproperForceComputeGPU class to python
void export_HarmonicImproperForceComputeGPU();

#endif
