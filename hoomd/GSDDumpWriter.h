// Copyright (c) 2009-2016 The Regents of the University of Michigan
// This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.


#ifndef __GSDDUMPWRITER_H__
#define __GSDDUMPWRITER_H__

#include "Analyzer.h"
#include "ParticleGroup.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include "hoomd/extern/gsd.h"

/*! \file GSDDumpWriter.h
    \brief Declares the GSDDumpWriter class
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

//! Analyzer for writing out GSD dump files
/*! GSDDumpWriter writes out the current state of the system to a GSD file
    every time analyze() is called. When a group is specified, only write out the
    particles in the group.

    On the first call to analyze() \a fname is created with a dcd header. If it already
    exists, append to the file (unless the user specifies overwrite=True).

    \ingroup analyzers
*/
class GSDDumpWriter : public Analyzer
    {
    public:
        //! Construct the writer
        GSDDumpWriter(boost::shared_ptr<SystemDefinition> sysdef,
                      const std::string &fname,
                      boost::shared_ptr<ParticleGroup> group,
                      bool overwrite=false,
                      bool truncate=false);

        //! Control attribute writes
        void setWriteAttribute(bool b)
            {
            m_write_attribute = b;
            }

        //! Control property writes
        void setWriteProperty(bool b)
            {
            m_write_property = b;
            }

        //! Control momentum writes
        void setWriteMomentum(bool b)
            {
            m_write_momentum = b;
            }

        //! Control topology writes
        void setWriteTopology(bool b)
            {
            m_write_topology = b;
            }

        //! Destructor
        ~GSDDumpWriter();

        //! Write out the data for the current timestep
        void analyze(unsigned int timestep);

    private:
        std::string m_fname;                //!< The file name we are writing to
        bool m_overwrite;                   //!< True if file should be overwritten
        bool m_truncate;                    //!< True if we should truncate the file on every analyze()
        bool m_is_initialized;              //!< True if the file is open
        bool m_write_attribute;             //!< True if attributes should be written
        bool m_write_property;              //!< True if properties should be written
        bool m_write_momentum;              //!< True if momenta should be written
        bool m_write_topology;              //!< True if topology should be written
        gsd_handle m_handle;                //!< Handle to the file

        boost::shared_ptr<ParticleGroup> m_group;   //!< Group to write out to the file

        //! Write a type mapping out to the file
        void writeTypeMapping(std::string chunk, std::vector< std::string > type_mapping);

        //! Initializes the output file for writing
        void initFileIO();

        //! Write frame header
        void writeFrameHeader(unsigned int timestep);

        //! Write particle attributes
        void writeAttributes(const SnapshotParticleData<float>& snapshot);

        //! Write particle properties
        void writeProperties(const SnapshotParticleData<float>& snapshot);

        //! Write particle momenta
        void writeMomenta(const SnapshotParticleData<float>& snapshot);

        //! Write bond topology
        void writeTopology(BondData::Snapshot& bond,
                           AngleData::Snapshot& angle,
                           DihedralData::Snapshot& dihedral,
                           ImproperData::Snapshot& improper,
                           ConstraintData::Snapshot& constraint);

        //! Check and raise an exception if an error occurs
        void checkError(int retval);
    };

//! Exports the GSDDumpWriter class to python
void export_GSDDumpWriter();

#endif
