from enum import Enum
from recordclass import dataobject
from kivy.event import EventDispatcher

class Mode(Enum):
    STEP = 0
    LINEAR = 1
    QUADRATIC_IN = 2
    QUADRATIC_OUT = 3
    SMOOTHSTEP = 4


class Node(dataobject):
    track: str
    value: float
    mode: Mode

    def to_dict(self):
        return {
            'track': self.track,
            'value': self.value,
            'mode': self.mode.value
        }

class Keyframe(EventDispatcher):
    def __init__(self, time: float, tracks: list[str], **kwargs):
        super().__init__(**kwargs)
        self.register_event_type('on_change')
        self.time = time
        self._nodes: list[Node] = [Node(track=t, value=0.0, mode=Mode.STEP) for t in tracks]
    
    def on_change(self, *args):
        """Default handler for the event"""
        pass

    @property
    def time(self) -> float:
        return self._time

    @time.setter
    def time(self, value: float):
        try:
            value = round(float(value), 2)
            if value < 0:
                raise ValueError("Time cannot be negative.")
            self._time = value
            self.dispatch("on_change")
        except (ValueError, TypeError) as e:
            raise ValueError(f"Invalid time value: {value}") from e
    
    @property
    def nodes(self) -> list[Node]:
        return self._nodes

    def set(self, track: str, value: float = None, mode: Mode = None):
        node = next((n for n in self._nodes if n.track == track), None)
        if node is None:
            raise KeyError(f"Track '{track}' does not exist.")

        changed = False
        if value is not None:
            try:
                node.value = float(value)
                changed = True
            except (ValueError, TypeError) as e:
                raise ValueError(f"Invalid float value: {value}") from e

        if mode is not None:
            if not isinstance(mode, Mode):
                raise ValueError(f"Invalid mode: {mode}")
            node.mode = mode
            changed = True

        if changed:
            self.dispatch("on_change")
    
    def copy_from(self, time: float, previous_keyframe: 'Keyframe'):
        """Copy properties from a previous keyframe into this one."""
        # Set the time for the new keyframe (may be a new time)
        self.time = time

        # Copy nodes from the previous keyframe, creating new Node instances
        self._nodes = []
        for prev_node in previous_keyframe.nodes:
            new_node = Node(track=prev_node.track, value=prev_node.value, mode=prev_node.mode)
            self._nodes.append(new_node)
            
        self.dispatch("on_change")
    
    def add_node(self, track_name: str, value: float = 0.0, mode: Mode = Mode.STEP):
        if any(n.track == track_name for n in self._nodes):
            raise ValueError(f"Track '{track_name}' already exists.")
        self._nodes.append(Node(track_name, value, mode))
        self.dispatch("on_change")

    def remove_node(self, track_name: str):
        if all(n.track != track_name for n in self._nodes):
            raise KeyError(f"Track '{track_name}' does not exist.")
        self._nodes = [n for n in self._nodes if n.track != track_name]
        self.dispatch("on_change")

    def to_dict(self) -> dict:
        return {
            "time": self.time,
            "nodes": [node.to_dict() for node in self._nodes],
        }