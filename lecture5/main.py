import cv2 as cv
import numpy as np

# --- Import the parts we need ---
# Simulation (fake robot and camera)
from sim.camera import Camera
from sim.robot import Robot
from sim.mover import OmniMover

# Aiming / tracking code
from aim.solver import Solver
from aim.tracker import TrackerByPose

# Helper tools
from utils.plotter import Plotter

# ==============================================================================
# Settings you can tune
# ==============================================================================

# 1. P0: how unsure we are about the starting state
# What we track: [x, vx, y, vy, z, vz,   a,   w, r]
P0_DIAG = [1, 64, 1, 64, 1, 64, 0.4, 100, 1]

# 2. Q: how much the robot's motion can surprise us (set using sigma, the standard deviation)
SIGMA_V = 100.0  # how much the moving speed can change unexpectedly (m/s^2)
SIGMA_W = 400.0  # how much the spinning speed can change unexpectedly (rad/s^2)

# 3. R: how noisy the camera measurements are
R_DIAG = [0.1, 0.1, 0.1, 0.1]

# ==============================================================================
# --- Simulation settings ---
T = 5
dt = 1 / 60
np.random.seed(42)

# --- Set up the simulation ---

# 1. Set up the robot and how it moves
robot = Robot(z0=0.1, h=0)
mover = OmniMover(
    angular_mode="sine_wave",
    p1=(3, 0),
    p2=(3, 0),
    v_linear=0,
    vel_amplitude=0,
    vel_frequency=np.pi,
    vel_center=6,
)

# 2. Set up the camera and the position solver
cam = Camera()
solver = Solver(cam.cam_mat, cam.dist_coeffs)

# 3. Set up the tracker (using the settings above)
tracker_cv = TrackerByPose(
    r0=robot.r,
    P0_diag=P0_DIAG,
    sigma_v=SIGMA_V,
    sigma_w=SIGMA_W,
    R_diag=R_DIAG,
)

# 4. Set up the plotting tool
plotter = Plotter()

# --- Main simulation loop ---
print("Simulation started. Press 'q' to quit...")
for t in np.arange(0, T, dt):
    # 1. Move the robot and get its true state
    mover.move(robot, dt)

    # 2. The camera looks at it and gives 2D picture points (with some noise)
    armor = robot.observe(cam)
    points = armor.project(cam)
    points += np.random.normal(0, 1.1, points.shape)

    # 3. The solver works out position and angle
    armor_yaw, armor_pos = solver.lsqr(points, cam.rot, cam.pos)

    # 4. The tracker smooths out the noise (filtering)
    tracker_cv.track(t, armor_yaw, armor_pos, armor.i)

    # --- Record data (plotted the same way as the full system) ---
    # Record the true values (x, vx, y, vy, yaw, w)
    plotter.add(
        t,
        robot.pos[0],
        mover.v[0],
        robot.pos[1],
        mover.v[1],
        robot.yaw,
        mover.w,
        name=("x_truth", "vx_truth", "y_truth", "vy_truth", "yaw_truth", "w_truth"),
    )

    # Record the full smoothed (filtered) state
    if tracker_cv.ekf is not None:
        plotter.add(
            t,
            *tracker_cv.ekf.x,
            name=("x", "vx", "y", "vy", "z", "vz", "yaw", "w", "r"),
            mode="lines+markers",
        )

    # --- Show it on screen ---
    img = cam.render(*robot.armors)
    cv.putText(img, f"t={t:.2f}s", (10, 30), 0, 1, (255, 255, 255))
    cv.polylines(img, [points.astype(int)], True, (0, 0, 255), 2)
    cv.imshow("Simulation View", img)
    if cv.waitKey(1) == ord("q"):
        break

# --- Show the results (this part was changed) ---
print("Simulation finished. Showing result charts...")
# Put all the variable names you want to plot in one tuple to show them on the same chart
plotter.show(
    (
        "x_truth",
        "x",
        "vx_truth",
        "vx",
        "y_truth",
        "y",
        "vy_truth",
        "vy",
        "yaw_truth",
        "yaw",
        "w_truth",
        "w",
    ),
)
cv.destroyAllWindows()
