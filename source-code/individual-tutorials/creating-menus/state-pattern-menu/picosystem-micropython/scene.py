class Scene:
    def __init__(self):
        self._next = None
        self._finished = False

    def enter(self):
        pass

    def leave(self):
        pass

    def update(self):
        pass

    def render(self):
        pass

    def next(self):
        return self._next

    def finished(self):
        return self._finished

    def finish(self, next_scene):
        self._next = next_scene
        self._finished = True