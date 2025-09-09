# Tunnel-Traffic-C-Threads-

  This project simulates a smart tunnel management system using multithreading in C with POSIX threads (pthreads) 

  The tunnel has multiple safety mechanisms (panic button, gas leak detection, smoke detection, external emergency control) that can restrict or stop car access when an incident occurs
  
  The simulation randomly generates events (cars entering/exiting, gas leaks, smoke, panic button presses, external control) and coordinates them using mutexes and condition variables to avoid race conditions and ensure correct synchronization

  Lines 1-20 (variable and library):
              -- MAX_CARS: Tunnel capacity (20 cars max)

              -- Mutexes: Ensure that shared variables (e.g., carCount, gasLeak, smokeDetected) are modified safely by one thread at a time

              -- Condition variables: Used to block or wake threads (e.g., when tunnel capacity is too high)

              -- State flags: Represent the tunnel’s current status (gas leak present, smoke detected, panic button pressed, external barrier active)

  Lines 21-71 (car_Monitoring_Entrance):  

               -- Cars can only enter if there’s no emergency (panic, gas, smoke, external block)

               -- Uses nested mutex locks to check all conditions safely

               -- If tunnel is full (≥ 20 cars) → block new entries until 50% of cars exit

               -- Uses condition variable tunelCapacity to wait until cars leave

  Lines 71-100 (car_Monitoring_Exit):

               -- Cars exit randomly (1–2 cars per cycle)

               -- If tunnel becomes empty, message is printed

               -- Signals waiting entrance thread (tunelCapacity) that space is available

  Lines 100-200 (functions for each interupt):

            Gas Leak
            
               -- Randomly simulates a gas leak.

               -- During leak → no cars can enter.

               -- Leak lasts ~6 seconds, then clears.

            Smoke detection 

               -- Randomly simulates smoke detection (fire event).

               -- Stops access for ~4 seconds until cleared.

            Panic Button 

               -- Simulates user panic button.

               -- Blocks tunnel access for ~5 seconds.

            External control for traffic 

               -- Simulates external authorities (police/fire dept.) blocking access.

               -- Tunnel is locked for ~5 seconds.
    

                














  
