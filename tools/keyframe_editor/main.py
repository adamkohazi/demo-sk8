from kivy.app import App

# Import directly from your components folder
from components.timeline.timeline import Timeline
from components.timeline.timeline_widget import TimelineEditor

class TimelineApp(App):
    def build(self):
        timeline = Timeline()

        timeline.add_track('color_R')
        timeline.add_track('color_G')
        timeline.add_track('color_B')
        timeline.add_keyframe(0.0)
        timeline.add_keyframe(1.0)

        timeline_widget = TimelineEditor()
        timeline_widget.timeline = timeline
        
        return timeline_widget

if __name__ == "__main__":
    TimelineApp().run()


"""
TODO: Label for keyframes
display prev and next keyframes
copy keyframe
"""