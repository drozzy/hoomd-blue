// Copyright (c) 2009-2016 The Regents of the University of Michigan
// This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.


// Maintainer: joaander

#include "hoomd/ForceCompute.h"
#include "hoomd/ParticleGroup.h"
#include <boost/shared_ptr.hpp>
#include "hoomd/extern/saruprng.h"
#include "hoomd/HOOMDMath.h"
#include "hoomd/VectorMath.h"

#include "EvaluatorConstraintEllipsoid.h"


/*! \file ActiveForceCompute.h
    \brief Declares a class for computing active forces
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

#ifndef __ACTIVEFORCECOMPUTE_H__
#define __ACTIVEFORCECOMPUTE_H__

//! Adds an active force to a number of particles
/*! \ingroup computes
*/
class ActiveForceCompute : public ForceCompute
    {
    public:
        //! Constructs the compute
        ActiveForceCompute(boost::shared_ptr<SystemDefinition> sysdef,
                             boost::shared_ptr<ParticleGroup> group,
                             int seed, boost::python::list f_lst,
                             bool orientation_link, Scalar rotation_diff,
                             Scalar3 P,
                             Scalar rx,
                             Scalar ry,
                             Scalar rz);

        //! Destructor
        ~ActiveForceCompute();

    protected:
        //! Actually compute the forces
        virtual void computeForces(unsigned int timestep);

        //! Set forces for particles
        virtual void setForces();

        //! Orientational diffusion for spherical particles
        virtual void rotationalDiffusion(unsigned int timestep);

        //! Set constraints if particles confined to a surface
        virtual void setConstraint();

        boost::shared_ptr<ParticleGroup> m_group;   //!< Group of particles on which this force is applied
        bool m_orientationLink;
        Scalar m_rotationDiff;
        Scalar m_rotationConst;
        Scalar3 m_P;          //!< Position of the Ellipsoid
        Scalar m_rx;          //!< Radius in X direction of the Ellipsoid
        Scalar m_ry;          //!< Radius in Y direction of the Ellipsoid
        Scalar m_rz;          //!< Radius in Z direction of the Ellipsoid
        int m_seed;           //!< Random number seed
        GPUArray<Scalar3> m_activeVec; //! active force unit vectors for each particle
        GPUArray<Scalar> m_activeMag; //! active force magnitude for each particle
        unsigned int last_computed;
    };

//! Exports the ActiveForceComputeClass to python
void export_ActiveForceCompute();
#endif
