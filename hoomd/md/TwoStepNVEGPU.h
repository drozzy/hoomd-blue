// Copyright (c) 2009-2016 The Regents of the University of Michigan
// This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.


// Maintainer: joaander

#include "TwoStepNVE.h"

#ifndef __TWO_STEP_NVE_GPU_H__
#define __TWO_STEP_NVE_GPU_H__

/*! \file TwoStepNVEGPU.h
    \brief Declares the TwoStepNVEGPU class
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

#include "hoomd/Autotuner.h"

//! Integrates part of the system forward in two steps in the NVE ensemble on the GPU
/*! Implements velocity-verlet NVE integration through the IntegrationMethodTwoStep interface, runs on the GPU

    \ingroup updaters
*/
class TwoStepNVEGPU : public TwoStepNVE
    {
    public:
        //! Constructs the integration method and associates it with the system
        TwoStepNVEGPU(boost::shared_ptr<SystemDefinition> sysdef, boost::shared_ptr<ParticleGroup> group);
        virtual ~TwoStepNVEGPU() {};

        //! Performs the first step of the integration
        virtual void integrateStepOne(unsigned int timestep);

        //! Performs the second step of the integration
        virtual void integrateStepTwo(unsigned int timestep);

        //! Set autotuner parameters
        /*! \param enable Enable/disable autotuning
            \param period period (approximate) in time steps when returning occurs
        */
        virtual void setAutotunerParams(bool enable, unsigned int period)
            {
            TwoStepNVE::setAutotunerParams(enable, period);
            m_tuner_one->setPeriod(period);
            m_tuner_one->setEnabled(enable);
            m_tuner_two->setPeriod(period);
            m_tuner_two->setEnabled(enable);
            }

    private:
        boost::scoped_ptr<Autotuner> m_tuner_one; //!< Autotuner for block size (step one kernel)
        boost::scoped_ptr<Autotuner> m_tuner_two; //!< Autotuner for block size (step two kernel)
    };

//! Exports the TwoStepNVEGPU class to python
void export_TwoStepNVEGPU();

#endif // #ifndef __TWO_STEP_NVE_GPU_H__
