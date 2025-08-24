/*******************************************************************************
 * File: main.c
 * Component: Flight Plan Engine (CI-AD432-002)
 * Version: 3.0.0
 * Date: August 24, 2024
 * 
 * Description: Main entry point for Flight Plan Engine
 * 
 * DO-178C Level: DAL-A
 * MISRA-C: 2012 Compliant
 * 
 * Copyright (c) 2024 AMEDEO-P Aerospace Defense and Ground Systems
 * Proprietary and Confidential
 *******************************************************************************/

#include "flightplan.h"
#include "navigation_db.h"
#include "performance.h"
#include "arinc429_interface.h"

/* Global flight plan context */
static FlightPlanContext_t g_flightPlanContext;

/* Function prototypes */
static void FlightPlanEngine_Initialize(void);
static void FlightPlanEngine_Shutdown(void);
static bool FlightPlanEngine_SelfTest(void);

/*******************************************************************************
 * Function: main
 * 
 * Description: Main entry point for Flight Plan Engine
 * 
 * Parameters: None
 * 
 * Returns: int32_t - Exit status (0 = success, non-zero = error)
 * 
 * Requirements Traced:
 *   HLR-FPE-001: Flight Plan Creation
 *   HLR-FPE-300: Fault Detection
 *******************************************************************************/
int32_t main(void)
{
    int32_t exitStatus = FPE_SUCCESS;
    bool selfTestPassed = false;
    
    /* Initialize flight plan engine */
    FlightPlanEngine_Initialize();
    
    /* Perform self-test */
    selfTestPassed = FlightPlanEngine_SelfTest();
    
    if (selfTestPassed)
    {
        /* Main processing loop */
        while (g_flightPlanContext.systemState == FPE_STATE_RUNNING)
        {
            /* Process incoming ARINC 429 messages */
            ARINC429_ProcessMessages();
            
            /* Update flight plan calculations if needed */
            FlightPlan_UpdateCalculations(&g_flightPlanContext);
            
            /* Check for system health */
            if (!FlightPlan_HealthCheck(&g_flightPlanContext))
            {
                /* Log error and transition to safe state */
                System_LogError(FPE_ERROR_HEALTH_CHECK_FAILED);
                g_flightPlanContext.systemState = FPE_STATE_SAFE;
                exitStatus = FPE_ERROR_HEALTH_CHECK_FAILED;
                break;
            }
            
            /* Yield control to ARINC 653 scheduler */
            OS_TaskDelay(FPE_MAIN_LOOP_DELAY_MS);
        }
    }
    else
    {
        /* Self-test failed */
        System_LogError(FPE_ERROR_SELF_TEST_FAILED);
        exitStatus = FPE_ERROR_SELF_TEST_FAILED;
    }
    
    /* Shutdown flight plan engine */
    FlightPlanEngine_Shutdown();
    
    return exitStatus;
}

/*******************************************************************************
 * Function: FlightPlanEngine_Initialize
 * 
 * Description: Initialize flight plan engine subsystems
 * 
 * Parameters: None
 * 
 * Returns: None
 * 
 * Requirements Traced:
 *   HLR-FPE-004: Database Integration
 *   HLR-FPE-201: Crew Interface
 *******************************************************************************/
static void FlightPlanEngine_Initialize(void)
{
    /* Initialize global context */
    memset(&g_flightPlanContext, 0, sizeof(FlightPlanContext_t));
    g_flightPlanContext.systemState = FPE_STATE_INITIALIZING;
    
    /* Initialize navigation database interface */
    NavigationDB_Initialize();
    
    /* Initialize performance calculation module */
    Performance_Initialize();
    
    /* Initialize ARINC 429 interface */
    ARINC429_Initialize();
    
    /* Load aircraft configuration */
    FlightPlan_LoadAircraftConfig(&g_flightPlanContext);
    
    /* System ready */
    g_flightPlanContext.systemState = FPE_STATE_RUNNING;
    
    System_LogInfo("Flight Plan Engine initialized successfully");
}

/*******************************************************************************
 * Function: FlightPlanEngine_Shutdown
 * 
 * Description: Shutdown flight plan engine and cleanup resources
 * 
 * Parameters: None
 * 
 * Returns: None
 *******************************************************************************/
static void FlightPlanEngine_Shutdown(void)
{
    g_flightPlanContext.systemState = FPE_STATE_SHUTDOWN;
    
    /* Shutdown subsystems in reverse order */
    ARINC429_Shutdown();
    Performance_Shutdown();
    NavigationDB_Shutdown();
    
    System_LogInfo("Flight Plan Engine shutdown complete");
}

/*******************************************************************************
 * Function: FlightPlanEngine_SelfTest
 * 
 * Description: Perform built-in self-test (BIST) of flight plan engine
 * 
 * Parameters: None
 * 
 * Returns: bool - true if all tests pass, false otherwise
 * 
 * Requirements Traced:
 *   HLR-FPE-300: Fault Detection
 *   HLR-FPE-301: Data Integrity
 *******************************************************************************/
static bool FlightPlanEngine_SelfTest(void)
{
    bool testResult = true;
    
    System_LogInfo("Starting Flight Plan Engine self-test");
    
    /* Test navigation database integrity */
    if (!NavigationDB_VerifyIntegrity())
    {
        System_LogError(FPE_ERROR_NAVDB_INTEGRITY);
        testResult = false;
    }
    
    /* Test performance calculation module */
    if (!Performance_SelfTest())
    {
        System_LogError(FPE_ERROR_PERFORMANCE_MODULE);
        testResult = false;
    }
    
    /* Test ARINC 429 interface */
    if (!ARINC429_SelfTest())
    {
        System_LogError(FPE_ERROR_ARINC429_INTERFACE);
        testResult = false;
    }
    
    /* Test flight plan calculation algorithms */
    if (!FlightPlan_AlgorithmTest())
    {
        System_LogError(FPE_ERROR_ALGORITHM_TEST);
        testResult = false;
    }
    
    if (testResult)
    {
        System_LogInfo("Flight Plan Engine self-test PASSED");
    }
    else
    {
        System_LogError("Flight Plan Engine self-test FAILED");
    }
    
    return testResult;
}

/* End of file */