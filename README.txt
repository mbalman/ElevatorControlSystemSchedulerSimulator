SUMMARY:
------------------------------------------------------------------------------------
Here I implement an elevator control system. Since this simulates a real elevator (lift) system, I made the following assumptions:

 - I assume it is online scheduling, requests are received at runtime, and there is no prior knowledge about a future request's arrival.

 - Except top and bottom floors, each floor has up and down buttons. A call request (pickup) has two attributes: floor id and up/down direction. Call requests are per floor.  

 - Inside each elevator, there are buttons for stop requests (drop offs). 

 - Each floor has displays for each elevator - showing current floor and whether the associated elevator is  going up or down. 

 - Passengers can take any elevator, and they can press any floor button to initiate a stop request (drop off) when they are inside at any time. 

 - If a call request is served but there are still passengers to be picked up, for example elevator is full, I assume passengers will press the call (up/down) button again. Elevator control system does have no knowledge about passengers.

 - For TESTING, a passenger simulator should be implemented (for calls at a certain time, and for stop requests when inside the elevator). Once a call (pickup) request is served, if ELEV_TEST define, a funtion is called to simulate a stop (drop off) request inside the elevator. I did not have time to implement the passenger simulator. Testing is also not complete.
 
 - Elevator scheduler could consider weigh, energy, difference in speed in going up or down, time spent to slow down before stopping, etc. For simplicity, I assume that an elevator proceeds one floor at each time step. Load/unload overhead, and time required to stop and accelerate are all ignored.  Assuming first floor's id is 0, there is no negative floor ids. 
 
OTHER DETAILS:
------------------------------------------------------------------------------------

 - Each elevator has a target floor to go (not necessarily a floor in the stop list). Scheduler can ask an elevator to go to a target. If target and current floors are same, that means staying idle.

 - Call request means pickup, comes from outside. Stop request means drop off, comes from inside.

 - Once a call request is scheduled, scheduler adds a new stop in the associated elevator's stopList ( stopList have drop off requests + scheduled pickup requests)

 - Each elevator have a capacity. Here I used the number of items in stopList to define elevator capacity. That is not the best representation because I assume only one passenger can come in and press a button inside. 

 - I keep only one stop list per elevator. There is no additional list for scheduled call requests. Logic here is that do not schedule a call (pick up) if you are already full.

 - At each time step, an elevator proceeds to the next floor towards its target, stops if the floor is in the stop list. Also, when its target becomes equal to current floor, it checks whether there is any call request in that floor. If there is in the same direction it is going, it takes. It takes an opposite direction call only when there is no stops in its stopList at its original direction.

 - An elevator goes up if there are higher floors in its stop lists. Or goes down if there are lower floors in its stop list.   Once it hits its target, it will change target in the opposite direction (if there is at least one item in the opposite direction).

 - I use intended direction. This is used internally to break ties. If elevator is going down, and when it reaches its target, intended direction is still down, even if it is idle (current==target). It will give preference to down calls (go down as much as you can first). Scheduler looks at this intended direction. 

SCHEDULER:
------------------------------------------------------------------------------------

 - Logic here is to go in one direction and pickup along the way. If there is no call or stop requests in that direction, and then process requests in other direction if there are any.

 - Can not go beyond top/bottom floor, so eventually need to go towards the opposite direction. That is how starvation is prevented. 

 - I want to do something smarter than checking call (pickup) requests every time an elevator proceeds to a new floor (in simulation time-stepping). Elevators know their target and next stop, so they do not continuously query. 

 - After any drop off or pickup (stopped at a floor), a new stop request came, or scheduler added a stop in the list,  elevator's target is recalculated. 

 - Scheduler is a central entity, can get info about capacity, current and target floors of each elevator, and can change target, and add to the stop list to schedule a call request. At every time step, it runs.

 - Scheduling a call request means: adding its floor id to an elevators stop list. Only schedule if call's direction and elevator's direction(intended) are same, elevator is below or at the same floor, and has capacity.

 - Scheduler selects the closest elevator. If they have same distance, it selects the one with least number of items in its stop list.

- If a call request can not be scheduled in the current cycle, it is delayed (will look again next time step). However, a delayed scheduling can still trigger an elevator to mobilize (changing its target). 

 - First process call requests both for all ups and downs, then go to the next phase. If a call request is not scheduled, we can mobilizing an elevator towards this call request (second phase). 

 - Mobilizing an elevator means updating the target of an elevator so it can come towards you. Mobilize an elevator if it is idle (target==current) and it has no item in its stop list. Also mobilize an elevator if it is coming towards you (but target is not passing you) so it can come to you floor.

 - When you schedule a call, you mark it as scheduled, so will not look again till a new call request comes at the same direction. A call request may or may not trigger to mobilize an elevator (if not scheduled). So in the next time stepping cycle, we look them again, try to schedule, or try to mobilize an elevator. It is lazy scheduling optimization (delaying the decision to come up with a better choice later).

 - It is hard to explain all in writing. Figures and examples will help to clarify it better !!! I put comments in the code which will also help understand the design.


INTERFACE:
------------------------------------------------------------------------------------

pickup(floor_id, up/down)  // outside
dropoff(elevator_id, goal_floor_id) // inside
step(step_number) // simulate n number of steps
dir(elevator_id)  // elevators direction (not the intended direction, it can return idle)
cur(elevator_id)  // elevators current floor

------------------------------------------------------------------------------------
