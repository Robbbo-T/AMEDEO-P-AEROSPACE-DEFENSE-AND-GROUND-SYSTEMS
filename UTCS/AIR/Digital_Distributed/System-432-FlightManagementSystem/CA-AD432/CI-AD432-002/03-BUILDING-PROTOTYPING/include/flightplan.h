/*******************************************************************************
 * File: flightplan.h
 * Component: Flight Plan Engine (CI-AD432-002)
 * Version: 3.0.0
 * Date: August 24, 2024
 * 
 * Description: Main header file for Flight Plan Engine
 * 
 * DO-178C Level: DAL-A
 * MISRA-C: 2012 Compliant
 * 
 * Copyright (c) 2024 AMEDEO-P Aerospace Defense and Ground Systems
 * Proprietary and Confidential
 *******************************************************************************/

#ifndef FLIGHTPLAN_H
#define FLIGHTPLAN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/*******************************************************************************
 * Constants and Macros
 *******************************************************************************/

/* Version information */
#define FPE_VERSION_MAJOR       (3U)
#define FPE_VERSION_MINOR       (0U)
#define FPE_VERSION_PATCH       (0U)

/* System constraints per HLR-FPE-101 */
#define FPE_MAX_MEMORY_BYTES    (4194304U)    /* 4 MB maximum */
#define FPE_MAX_RESPONSE_TIME_MS (50U)        /* 50 ms per HLR-FPE-100 */
#define FPE_MAIN_LOOP_DELAY_MS  (10U)        /* 10 ms loop period */

/* Flight plan constraints */
#define FPE_MAX_WAYPOINTS       (99U)         /* Maximum waypoints per flight plan */
#define FPE_MAX_ROUTE_NAME_LEN  (8U)          /* Route name length per ARINC 424 */
#define FPE_MAX_AIRPORT_ID_LEN  (4U)          /* Airport identifier length */

/* Calculation accuracy per HLR-FPE-102 */
#define FPE_DISTANCE_ACCURACY_NM (0.1)        /* ±0.1 nautical miles */
#define FPE_FUEL_ACCURACY_PERCENT (1.0)       /* ±1% fuel estimates */

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/

/* System state enumeration */
typedef enum
{
    FPE_STATE_UNINITIALIZED = 0,
    FPE_STATE_INITIALIZING,
    FPE_STATE_RUNNING,
    FPE_STATE_SAFE,
    FPE_STATE_SHUTDOWN
} FlightPlanState_t;

/* Error codes */
typedef enum
{
    FPE_SUCCESS = 0,
    FPE_ERROR_INVALID_PARAMETER,
    FPE_ERROR_MEMORY_ALLOCATION,
    FPE_ERROR_NAVDB_INTEGRITY,
    FPE_ERROR_PERFORMANCE_MODULE,
    FPE_ERROR_ARINC429_INTERFACE,
    FPE_ERROR_ALGORITHM_TEST,
    FPE_ERROR_HEALTH_CHECK_FAILED,
    FPE_ERROR_SELF_TEST_FAILED,
    FPE_ERROR_CALCULATION_TIMEOUT
} FlightPlanError_t;

/* Geographic position structure */
typedef struct
{
    double latitude_deg;    /* Latitude in degrees (-90 to +90) */
    double longitude_deg;   /* Longitude in degrees (-180 to +180) */
    int32_t altitude_ft;    /* Altitude in feet MSL */
} Position_t;

/* Waypoint structure per ARINC 424 */
typedef struct
{
    char identifier[6];     /* Waypoint identifier (5 chars + null) */
    Position_t position;    /* Geographic position */
    uint16_t course_deg;    /* Course to waypoint (0-359 degrees) */
    uint32_t distance_nm;   /* Distance to waypoint (nautical miles) */
    bool isValid;           /* Waypoint validity flag */
} Waypoint_t;

/* Flight plan leg structure */
typedef struct
{
    Waypoint_t fromWaypoint;    /* Departure waypoint */
    Waypoint_t toWaypoint;      /* Arrival waypoint */
    uint32_t distance_nm;       /* Leg distance (nautical miles) */
    uint32_t time_sec;          /* Estimated time (seconds) */
    uint32_t fuel_lbs;          /* Estimated fuel consumption (pounds) */
    uint16_t track_deg;         /* True track (0-359 degrees) */
} FlightPlanLeg_t;

/* Flight plan structure */
typedef struct
{
    char originAirport[FPE_MAX_AIRPORT_ID_LEN + 1];     /* Origin airport ICAO code */
    char destinationAirport[FPE_MAX_AIRPORT_ID_LEN + 1]; /* Destination airport ICAO code */
    Waypoint_t waypoints[FPE_MAX_WAYPOINTS];            /* Waypoint array */
    FlightPlanLeg_t legs[FPE_MAX_WAYPOINTS - 1];        /* Flight plan legs */
    uint8_t waypointCount;                              /* Number of active waypoints */
    uint32_t totalDistance_nm;                          /* Total route distance */
    uint32_t totalTime_sec;                             /* Total estimated time */
    uint32_t totalFuel_lbs;                             /* Total estimated fuel */
    bool isActive;                                      /* Flight plan active flag */
} FlightPlan_t;

/* Aircraft configuration structure */
typedef struct
{
    char aircraftType[8];       /* Aircraft type (e.g., "B787") */
    uint32_t maxFuel_lbs;       /* Maximum fuel capacity */
    uint16_t cruiseSpeed_kts;   /* Cruise speed (knots) */
    int32_t cruiseAltitude_ft;  /* Cruise altitude (feet) */
    uint16_t fuelFlow_pph;      /* Fuel flow rate (pounds per hour) */
} AircraftConfig_t;

/* Main flight plan engine context */
typedef struct
{
    FlightPlanState_t systemState;      /* Current system state */
    FlightPlan_t activeFightPlan;       /* Active flight plan */
    AircraftConfig_t aircraftConfig;    /* Aircraft configuration */
    uint32_t lastCalculationTime_ms;    /* Last calculation timestamp */
    uint32_t memoryUsage_bytes;         /* Current memory usage */
    bool healthStatus;                  /* System health status */
} FlightPlanContext_t;

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/* Flight plan management functions */
bool FlightPlan_Create(FlightPlanContext_t* context, 
                       const char* origin, 
                       const char* destination);

bool FlightPlan_AddWaypoint(FlightPlanContext_t* context, 
                           const Waypoint_t* waypoint, 
                           uint8_t insertIndex);

bool FlightPlan_RemoveWaypoint(FlightPlanContext_t* context, 
                              uint8_t waypointIndex);

bool FlightPlan_UpdateCalculations(FlightPlanContext_t* context);

/* System functions */
bool FlightPlan_LoadAircraftConfig(FlightPlanContext_t* context);
bool FlightPlan_HealthCheck(const FlightPlanContext_t* context);
bool FlightPlan_AlgorithmTest(void);

/* Utility functions */
uint32_t FlightPlan_CalculateDistance(const Position_t* pos1, const Position_t* pos2);
uint16_t FlightPlan_CalculateBearing(const Position_t* from, const Position_t* to);
bool FlightPlan_ValidatePosition(const Position_t* position);

/* Logging functions */
void System_LogInfo(const char* message);
void System_LogError(FlightPlanError_t errorCode);

/* OS interface functions */
void OS_TaskDelay(uint32_t delayMs);

#endif /* FLIGHTPLAN_H */

/* End of file */