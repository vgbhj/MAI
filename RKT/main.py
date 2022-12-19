import krpc
import time

conn = krpc.connect()
vessel = conn.space_center.active_vessel

while True:
    print('(%.1f, %.1f, %.1f)' % vessel.position(vessel.orbit.body.reference_frame))
    time.sleep(1)