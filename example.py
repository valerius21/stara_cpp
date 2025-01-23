import numpy as np
from stara_cpp import load_maze, AStar


def main():
    # Create a sample maze (0 = wall, 1 = path)
    maze = np.array(
        [
            [1, 0, 1, 1, 1],
            [1, 1, 1, 0, 1],
            [0, 0, 1, 0, 1],
            [1, 1, 1, 0, 1],
            [1, 0, 1, 1, 1],
        ],
        dtype=np.int32,
    )

    maze_ptr = load_maze(maze)
    pathfinder = AStar(maze_ptr)

    start = (0, 0)
    goal = (4, 4)

    try:
        path = pathfinder.find_path(start, goal)
        if path is not None:
            print("Path found:", path)
        else:
            print("No path found!")
    except RuntimeError as e:
        print("Error finding path:", str(e))


if __name__ == "__main__":
    main()
