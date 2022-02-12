
/*
 * version.h
 *
 *	Copyright 2021 Denis Davydov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */



#ifndef _VERSION_H_
#define _VERSION_H_

#include "stconfig.h"


#ifdef __ORBIS_AIR__
#define TARGET "orbis-air" 
#define FW_FIRMWARE_NAME            "Orbis Air"
#define FW_FIRMWARE_IDENTIFIER      "OrbAr"
#endif

#ifdef __ORBIS_SURFACE__
#define TARGET "orbis-surface" 
#define FW_FIRMWARE_NAME            "Orbis Surface"
#define FW_FIRMWARE_IDENTIFIER      "OrbSfr"
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


#define FW_VERSION_MAJOR            1  // increment when a major release is made (big new feature, etc)
#define FW_VERSION_MINOR            3  // increment when a minor release is made (small new feature, change etc)
#define FW_VERSION_PATCH_LEVEL      2  // increment when a bug is fixed

#define FW_VERSION_STRING STR(FW_VERSION_MAJOR) "." STR(FW_VERSION_MINOR) "." STR(FW_VERSION_PATCH_LEVEL)

#define PCB_VERSION_MAJOR            1  // increment when a major release is made (big new feature, etc)
#define PCB_VERSION_MINOR            0  // increment when a minor release is made (small new feature, change etc)
#define PCB_VERSION_PATCH_LEVEL      1  // increment when a bug is fixed

#define PCB_VERSION_STRING STR(PCB_VERSION_MAJOR) "." STR(PCB_VERSION_MINOR) "." STR(PCB_VERSION_PATCH_LEVEL)

#endif  /* _VERSION_H_ */