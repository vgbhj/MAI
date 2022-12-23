import krpc
import time

conn = krpc.connect()
vessel = conn.space_center.active_vessel
print(vessel.name)
srf_frame = vessel.orbit.body.reference_frame

f = open("flight.log", "a")

while True:
    mass = int(vessel.mass)
    dry_mass = int(vessel.dry_mass)
    specific_impulse = vessel.specific_impulse
    vacuum_specific_impulse = vessel.vacuum_specific_impulse
    mission_time = int(vessel.met)
    altitude = int(vessel.flight().mean_altitude)
    thrust = vessel.thrust/1000
    f.write(f"Time: {mission_time//3600}h {mission_time//60}m {mission_time%60}s\n")
    f.write(f"    Mass: {mass} T   Dry mass: {dry_mass} T\n")
    f.write(f"    Thrust: {round(thrust,1)} kN\n")
    f.write(f"    Speed: {round(vessel.flight(srf_frame).speed, 1)} m/s\n")
    f.write(f"    Specific impulse: {round(specific_impulse, 1)}\n")
    f.write(f"    Vacuum specific impulse: {round(vacuum_specific_impulse, 1)}\n")
    f.write(f"    Altitude: {int(vessel.flight(srf_frame).surface_altitude)} m\n")
    f.write(f"    TWR: {round(thrust*1000/mass/9.8,2)}\n")
    time.sleep(1)