//----------------------------------------------------------------------------//
// file description
// 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// APP:  application layer code
// 
// Third-party compatible softward


//----------------------------------------------------------------------------//
Directory description
//----------------------------------------------------------------------------
The directory name is the abbreviation of a sub-module. The description is as follows:
demo: application sample code
module_app: sample code of application
                nv: NV parameter configuration file
                src: APP sample code



//----------------------------------------------------------------------------//
NV parameter configuration file
//----------------------------------------------------------------------------
Default nv configuration directory at the application layer of each sub-module: "project-directory/app/sub-module/nv/bbit_common","project-directory/app/sub-module/nv/outside_common";
Default NV configuration directory at the bottom of each sub module: "project-directory/nv/src/sub-module-name_common";

There may be multiple subversions under each submodule according to the requirements.
If there is a difference between the NV configuration related to the application layer of the subversion and the default configuration,
follow the three steps below:
    1) Create a configuration directory for the subversion£º"project-directory/app/sub-module/nv/bbit_subversion-name","project-directory/app/sub-module/nv/outside_subversion-name"£»
    2) Copy the files(cfg directory and app_nvi_db.xml) in the default configuration directory to the new directory.
    3) Add or modify configuration£»

If there is a difference between the underlying NV configuration of the subversion and the default configuration, follow the three steps below:
    1) Create a configuration directory for the subversion: "project-directory/nv/src/sub-module-name_sub-version-name";
    2) Copy the files(mss_nvi_db.xml) in the default configuration directory to the new directory;
    3) Add or modify configuration£»

