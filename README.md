# sp_vision_tutorial_26 (English)

Starter code for new members of the vision team, 2026 season.

This is a plain-English translation of
[TongjiSuperPower/sp_vision_tutorial_26](https://github.com/TongjiSuperPower/sp_vision_tutorial_26)
(commit `2bb6179`). Only the comments and on-screen messages were translated. The code works the same as the original.

Lecture 1: Hello Ubuntu & git  
Lecture 2: Hello C++  
Lecture 3: Hello OOP  
Lecture 4: Hello Armor ([homework directions](lecture4/homework/README.md))  
Lecture 5: Hello Kalman  

All future lecture videos will be posted (in Chinese) on the team's official Bilibili account: [TJ-SuperPower team](https://space.bilibili.com/651837546)

## Words you will see

These lessons are for RoboMaster, a robot competition. Robots try to hit each other's armor plates.

- **Armor plate**: a panel on an enemy robot that you aim at. It has a number or picture on it.
- **Light bar**: one of the two glowing strips on each side of an armor plate. The code finds these first, then finds the plate between them.
- **Sentry**: a robot type that guards an area.
- **Buff (power rune)**: a spinning target in the middle of the field. Hitting it gives your team a bonus (Lecture 4 homework).
- **Gimbal**: the motorized mount that turns the robot's camera and launcher.
- **Yaw / pitch / roll**: turning left-right / tilting up-down / tilting sideways.
- **Kalman filter (EKF)**: a way to smooth out noisy measurements and guess where a moving target will be next (Lecture 5).

## Not translated

The camera maker's files in `io/hikrobot/include/` were left as they are. They are Hikrobot's own files, and they already have English next to the Chinese.

## License

MIT, same as the original. See [LICENSE](LICENSE).
