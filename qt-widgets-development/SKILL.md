---
name: qt-widgets-development
description: Build or extend Qt 6 Widgets desktop interfaces using AlexKilim's shared widgets, Fusion palette and icons. Apply instrument-display rules when the screen presents measurements, equipment state or commands.
---

For a Qt Widgets project, inspect the existing theme and component usage. Consult the [qt-widgets catalog](https://github.com/AlexKilim/qt-widgets) and its gallery before implementing a matching component. When the application uses a library component, link the CMake target `akwidgets::widgets` at a pinned revision. Reuse `StatusChip` for compact labeled status fields. For a new app without an established theme, call `akwidgets::theme::apply()` after constructing QApplication and before creating widgets or plots. Let widgets inherit the application palette.

Choose colors by role. Give each color-coded state a visible cue that distinguishes it, such as a label, a different shape, or a slash across a blocked antenna. Supplement the visible cue with an accessible name or description for assistive technology. Use the existing gallery artwork for issue severity, recording, connectivity, antenna, satellites and Ethernet. Keep protocol decoding, thresholds and device-specific state descriptions in the consuming application. Red Tx and green Rx describe radio direction; select severity separately.

Keep the interface compact and easy to scan. Keep primary actions and important states visible. Add short hover tooltips to icon-only buttons and controls whose purpose is unclear. Use tooltips for supplementary explanations and show information needed to operate the interface directly on screen.

For mutually exclusive actions on the same process, use one button when its label clearly identifies the next action. For example, change Start to Stop after startup is confirmed. Show a pending state while the command is in progress. Use separate buttons when the actions need to be available independently.

For live measurements, distinguish zero, missing, stale and last measured values. Derive the freshness limit from the relevant data stream; unrelated traffic cannot refresh it. Keep command acceptance distinct from confirmation by fresh equipment state. On reconnect, clear pending actions and activity baselines. Verify the states relevant to the change, including loss and recovery of data, using an offscreen interaction test where practical.

Use the existing project's preferences when extending a screen. A candidate for changing the shared library or these instructions goes through user confirmation before editing or publication.
