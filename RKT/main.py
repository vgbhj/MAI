import krpc
import time

conn = krpc.connect()
vessel = conn.space_center.active_vessel
print(vessel.name)
srf_frame = vessel.orbit.body.reference_frame

while True:
    mass = int(vessel.mass)
    dry_mass = int(vessel.dry_mass)
    specific_impulse = vessel.specific_impulse
    vacuum_specific_impulse = vessel.vacuum_specific_impulse
    mission_time = int(vessel.met)
    altitude = int(vessel.flight().mean_altitude)
    thrust = vessel.thrust/1000
    print(f"Time: {mission_time//3600}h {mission_time//60}m {mission_time%60}s")
    print(f"    Mass: {mass} T   Dry mass: {dry_mass} T")
    print(f"    Thrust: {round(thrust,1)} kN")
    print(f"    Speed: {round(vessel.flight(srf_frame).speed, 1)} m/s")
    print(f"    Specific impulse: {round(specific_impulse, 1)}")
    print(f"    Vacuum specific impulse: {round(vacuum_specific_impulse, 1)}")
    print(f"    Altitude: {int(vessel.flight(srf_frame).surface_altitude)} m")
    print(f"    TWR: {thrust/mass/9.8}")
    time.sleep(1)