import numpy as np
from stara_cpp import load_maze, find_path


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

    # Load the maze into the C++ implementation
    maze_ptr = load_maze(maze)

    # Find a path from (0,0) to (4,4)
    start = (0, 0)
    goal = (4, 4)

    try:
        path = find_path(maze_ptr, start, goal)
        print("Path found:", path)
    except RuntimeError as e:
        print("Error finding path:", str(e))


if __name__ == "__main__":
    main()
