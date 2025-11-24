import os
import json
import pandas
from kivy.event import EventDispatcher
from components.keyframe.keyframe import Keyframe, Mode

class Timeline(EventDispatcher):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.register_event_type('on_change')

        self._time = 0.0
        self._tracks = []
        self._keyframes = []
    
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
    def tracks(self):
        return list(self._tracks)

    @property
    def keyframes(self):
        return sorted(self._keyframes, key=lambda kf: kf.time)

    def add_track(self, track_name: str):
        if track_name not in self._tracks:
            self._tracks.append(track_name)
            for keyframe in self._keyframes:
                keyframe.add_node(track_name)
            self.dispatch('on_change')
    
    def remove_track(self, track_name: str):
        if track_name in self._tracks:
            self._tracks.remove(track_name)
            for keyframe in self._keyframes:
                keyframe.remove_node(track_name)
            self.dispatch('on_change')

    def get_keyframe(self, time: float = None):
        key = round(float(time), 2) if time is not None else self.time
        return next((kf for kf in self._keyframes if kf.time == key), None)

    def add_keyframe(self, time: float = None):
        key = round(float(time), 2) if time is not None else self.time
        if all(kf.time != key for kf in self._keyframes):
            # Find the previous keyframe (if it exists)
            previous_keyframe = None
            for kf in reversed(self._keyframes):
                if kf.time < key:
                    previous_keyframe = kf
                    break
            
            # Create a copy of the previous keyframe
            if previous_keyframe:
                new_keyframe = Keyframe(key, self._tracks)
                new_keyframe.copy_from(key, previous_keyframe)
            
            # Create a new default keyframe
            else:
                new_keyframe = Keyframe(key, self._tracks)

            self._keyframes.append(new_keyframe)
            self.dispatch('on_change')
    
    def remove_keyframe(self, time: float = None):
        key = round(float(time), 2) if time is not None else self.time
        keyframe = self.get_keyframe(key)
        if keyframe:
            self._keyframes.remove(keyframe)
            self.dispatch('on_change')

    def export_json(self, filename: str, fixsize: int = None):
        try:
            data = {
                'time': self._time,
                'tracks': self._tracks,
                'keyframes': [kf.to_dict() for kf in self.keyframes]
            }

            # Pad or truncate to fixsize
            if fixsize is not None:
                keyframes = self.keyframes
                padded_keyframes = [
                    keyframes[min(len(keyframes) - 1, i)].to_dict()
                    for i in range(fixsize)
                ]
                data['keyframes'] = padded_keyframes

            with open(filename, 'w', encoding='utf-8') as f:
                json.dump(data, f, ensure_ascii=False, indent=4)
            print(f"Data successfully exported to {filename}")
        except Exception as e:
            raise(f"Failed to export JSON: {e}")

    def import_json(self, filename):
        try:
            with open(filename, 'r', encoding='utf-8') as f:
                data = json.load(f)
            
            self._time = float(data.get('time', 0.0))
            self._tracks = data.get('tracks', [])
            self._keyframes = []
            
            for kf_data in data.get('keyframes', []):
                time = float(kf_data['time'])
                if time not in {kf.time for kf in self._keyframes}:
                    new_kf = Keyframe(time, self._tracks)
                    for node in kf_data.get('nodes', []):
                        new_kf.set(
                            track=node['track'],
                            value=float(node['value']),
                            mode=Mode(node['mode']))
                    self._keyframes.append(new_kf)

            self.dispatch('on_change')

        except Exception as e:
            raise(f"Failed to import JSON: {e}")

    def export_excel(self, filename: str):
        """
        Export to Excel where:
            - Rows = tracks
            - Columns = keyframes (time values)
            - Cell values = node.value for each track in each keyframe
        """
        try:
            # Extract time values for columns
            column_labels = []
            keyframe_dicts = []
            for kf in self.keyframes:
                kfd = kf.to_dict()
                time_val = kfd.get("time", None)
                column_labels.append(time_val)
                keyframe_dicts.append(kfd)

            # Create empty DataFrame: one row per track, one column per keyframe
            df = pandas.DataFrame(index=self._tracks, columns=column_labels)

            # Fill DataFrame with node values
            for col_label, kfd in zip(column_labels, keyframe_dicts):
                for node in kfd.get("nodes", []):
                    track = node.get("track")
                    if track in self._tracks:
                        df.at[track, col_label] = node.get("value")

            # Export to Excel
            df.to_excel(filename, index=True)
            print(f"Data successfully exported to {filename}")

        except Exception as e:
            raise Exception(f"Failed to export Excel: {e}")

    def export_header(self, filename: str):
        """
        Export keyframe tracks to a C/C++ header, where each track becomes:

            constexpr Keyframe trackname[] = {
                { TIME, VALUE, MODE },
                ...
            };
        """

        try:
            with open(filename, "w", encoding="utf-8") as f:
                # Header notice
                f.write('// This header is generated by the keyframe editor.\n\n')

                # Build arrays per track name
                for track_name in self.tracks:

                    # Collect all node entries for this track across all keyframes
                    entries = []
                    for kf in self.keyframes:
                        for node in kf.nodes:
                            if node.track == track_name:
                                entries.append((kf.time, node.value, node.mode.name))

                    # Start array for this track
                    f.write(f"constexpr Keyframe<float> {track_name}[] = {{\n")

                    # Write rows
                    for (t, v, m) in entries:
                        f.write(f"    {{ {t}, {v}, {m} }},\n")

                    f.write("};\n\n")

            print(f"Header successfully exported to {filename}")

        except Exception as e:
            raise RuntimeError(f"Failed to export header: {e}")