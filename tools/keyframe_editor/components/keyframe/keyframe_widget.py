from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.uix.label import Label
from kivy.uix.spinner import Spinner
from kivy.uix.button import Button
from kivy.properties import ObjectProperty
from kivy.app import App

from components.timeline.timeline import Mode, Keyframe

class KeyframeEditor(BoxLayout):
    keyframe = ObjectProperty(None)

    def on_keyframe(self, instance, value):
        if isinstance(value, Keyframe):
            self.update()
            value.bind(on_change=self.on_keyframe_change)
    
    def on_keyframe_change(self, *args):
        # Tell parent about the change
        print('keyframe changed.')
        app = App.get_running_app()
        if hasattr(app.root, "update"):
            app.root.update()

    def update(self):
        if not self.keyframe:
            return
        
        # Update time
        self.ids.time_input.text = str(self.keyframe.time)

        # Clear tracks and keyframes
        self.ids.keyframe_nodes.clear_widgets()

        # Only proceed if timeline is assigned
        if not self.keyframe:
            return
        
        # Display tracks
        for node in self.keyframe.nodes:
            row = BoxLayout(
                orientation='horizontal',
                size_hint_y=None,
                height="30dp"
                )

            # Track name
            row.add_widget(Label(
                size_hint_x=None,
                width=150,
                text=node.track
            ))

            # Value
            value_input = TextInput(
                size_hint_x=None,
                width=50,
                text=str(node.value),
                multiline=False
            )

            def on_value_change(instance, text, track=node.track):
                try:
                    self.keyframe.set(track, value=text)
                except Exception as e:
                    print(f"Error updating value for '{track}': {e}")

            value_input.bind(text=on_value_change)
            

            # Buttons to increment and decrement the value by 0.1
            def decrement_value(instance, track=node.track):
                try:
                    current_value = float(value_input.text)
                    new_value = current_value - 0.1
                    value_input.text = str(round(new_value, 1))  # Update the value input
                    self.keyframe.set(track, value=str(round(new_value, 1)))  # Set the new value
                    self.update()
                except ValueError:
                    print(f"Error decrementing value for '{track}': Invalid number format")

            def increment_value(instance, track=node.track):
                try:
                    current_value = float(value_input.text)
                    new_value = current_value + 0.1
                    value_input.text = str(round(new_value, 1))  # Update the value input
                    self.keyframe.set(track, value=str(round(new_value, 1)))  # Set the new value
                    self.update()
                except ValueError:
                    print(f"Error incrementing value for '{track}': Invalid number format")

            
            # - button to decrement the value
            decrement_button = Button(
                text='-',
                size_hint_x=None,
                width=40
            )
            decrement_button.bind(on_press=decrement_value)
            
            
            # + button to increment the value
            increment_button = Button(
                text='+',
                size_hint_x=None,
                width=40
            )
            increment_button.bind(on_press=increment_value)

            row.add_widget(decrement_button)
            row.add_widget(value_input)
            row.add_widget(increment_button)

            # Mode
            mode_spinner = Spinner(
                size_hint_x=None,
                width=150,
                text=str(node.mode.name),
                values=[m.name for m in Mode]
            )

            def on_mode_change(instance, selected_mode, track=node.track):
                try:
                    self.keyframe.set(track, mode=Mode[selected_mode])
                except Exception as e:
                    print(f"Error updating mode for '{track}': {e}")

            # Bind with captured time
            mode_spinner.bind(text=on_mode_change)
            row.add_widget(mode_spinner)

            self.ids.keyframe_nodes.add_widget(row)

        
