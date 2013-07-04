dt = 1.0 / 60 / 5 # aka framerate
t = 10.0      # seconds

da = 4.5      # da/dt
a = 9.8       # gravity
v = 0.0       # start velocity

real_s = v * t + 0.5 * a * t ** 2
# real_s = 0.5 * a * t ** 2 + 1.0 / 6 * da * t ** 3

_steps = int(t / dt)
print "physics steps =", _steps

physics_s = 0
for i in range(_steps):
    # a = a + da * dt
    v = v + a * dt
    physics_s = physics_s + v * dt

print "real distance =", real_s
print "physics distance =", physics_s
