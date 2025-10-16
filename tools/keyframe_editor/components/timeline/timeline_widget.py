import os
from kivy.lang import Builder
from kivy.properties import ObjectProperty, NumericProperty
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.popup import Popup
from kivy.uix.textinput import TextInput
from kivy.uix.label import Label
from kivy.uix.filechooser import FileChooserListView

from components.timeline.timeline import Timeline
from components.keyframe.keyframe_widget import KeyframeEditor


kv_path = os.path.join(os.path.dirname(__file__), 'timeline_widget.kv')
Builder.load_file(kv_path)

class TimelineEditor(BoxLayout):
    timeline = ObjectProperty(None)

    min = NumericProperty(0.0)
    max = NumericProperty(10.0)
    step = NumericProperty(0.01)

    _updating = False

    def on_timeline(self, instance, value):
        if isinstance(value, Timeline):
            value.bind(on_change=self.on_timeline_change)
            self.update()
    
    def on_timeline_change(self, *args):
        print('Timeline changed.')
        self.update()

    def update(self):
        if self._updating or not self.timeline:
            return
        
        self._updating = True
        try:
            print("Updating timeline.")

            # Clear tracks and keyframes
            self.ids.keyframe_container.clear_widgets()

            # Update time display and slider
            self.ids.time_label.text = str(self.timeline.time)
            self.ids.time_slider.value = str(self.timeline.time)
            
            # Display keyframes
            for keyframe in self.timeline.keyframes:
                btn = Button(
                    text=f"{keyframe.time:.2f}s",
                    size_hint_x=None,
                    width=80,
                    background_color=(0, 0, 1, 1) if abs(keyframe.time - self.timeline.time) < 1e-5 else (1, 1, 1, 1)
                )

                # Bind with captured time
                btn.bind(on_press=lambda instance, t=keyframe.time: setattr(self.timeline, 'time', t))
                self.ids.keyframe_container.add_widget(btn)
            
            # Update keyframe editor
            keyframe = self.timeline.get_keyframe(self.timeline.time)
            if keyframe:
                self.ids.keyframe_editor.keyframe = keyframe

            # Autosave
            if self.ids.autosave_checkbox.active:
                self.export_file(self.ids.file_path_input.text)

        finally:
            self._updating = False

    def add_track_prompt(self):
        content = BoxLayout(orientation='vertical', spacing=10, padding=10)
        input_box = TextInput(hint_text='Track Name', multiline=False)
        content.add_widget(input_box)

        buttons = BoxLayout(size_hint_y=None, height='40dp', spacing=10)
        btn_ok = Button(text='Add', size_hint_x=0.5)
        btn_cancel = Button(text='Cancel', size_hint_x=0.5)
        buttons.add_widget(btn_ok)
        buttons.add_widget(btn_cancel)
        content.add_widget(buttons)

        popup = Popup(title='Add Track', content=content,
                      size_hint=(0.5, 0.3), auto_dismiss=False)

        def add_track_and_close(instance):
            track_name = input_box.text.strip()
            if track_name:
                self.timeline.add_track(track_name)
            popup.dismiss()

        btn_ok.bind(on_press=add_track_and_close)
        btn_cancel.bind(on_press=lambda x: popup.dismiss())

        popup.open()
    
    def open_filechooser(self, text_input):
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)
        chooser = FileChooserListView(path='.', filters=['*.json'], size_hint=(1, 0.9))
        layout.add_widget(chooser)

        buttons = BoxLayout(size_hint_y=None, height='40dp', spacing=10)
        btn_select = Button(text='Select')
        btn_cancel = Button(text='Cancel')
        buttons.add_widget(btn_select)
        buttons.add_widget(btn_cancel)
        layout.add_widget(buttons)

        popup = Popup(title='Select JSON File', content=layout,
                    size_hint=(0.9, 0.9), auto_dismiss=False)

        def select_file(instance):
            if chooser.selection:
                text_input.text = chooser.selection[0]
            popup.dismiss()

        btn_select.bind(on_press=select_file)
        btn_cancel.bind(on_press=lambda x: popup.dismiss())
        popup.open()
    
    def import_file(self, filepath):
        if filepath and os.path.exists(filepath):
            self.timeline.importJSON(filepath)
        else:
            print("Invalid import path")

    def export_file(self, filepath):
        if filepath:
            if not filepath.endswith('.json'):
                filepath += '.json'
            self.timeline.exportJSON(filepath, 255)
            print(f"Exported to {filepath}")
        else:
            print("Please enter a valid export path")