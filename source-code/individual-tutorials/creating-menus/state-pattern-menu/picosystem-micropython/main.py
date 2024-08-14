from scene import Scene
from game_scenes import TitleScene

# We still need to declare this in the global scope, but it's the only variable which we need to do this for
current_scene = TitleScene()
current_scene.enter()

def update(tick):
    global current_scene
    
    current_scene.update()

    # Check if we need to change scenes
    if current_scene.finished():
        next_scene = current_scene.next()

        # Clean up the old scene
        current_scene.leave()

        # Change the scene
        current_scene = next_scene
        current_scene.enter()

def draw(tick):
    current_scene.render()

# Start the game
start()