---
name: qt-widgets-development
description: Build or extend Qt 6 Widgets desktop interfaces using AlexKilim's shared widgets, Fusion palette and icons. Apply instrument-display rules when the screen presents measurements, equipment state or commands.
---

For a Qt Widgets project, inspect the existing theme and component usage. Consult the [qt-widgets catalog](https://github.com/AlexKilim/qt-widgets) and its gallery before implementing a matching component. When the application uses a library component, link the CMake target `akwidgets::widgets` at a pinned revision. Reuse `StatusChip` for compact labeled status fields. For a new app without an established theme, call `akwidgets::theme::apply()` after constructing QApplication and before creating widgets or plots. Let widgets inherit the application palette.

Choose colors by role. Give each color-coded state a visible cue that distinguishes it, such as a label, a different shape, or a slash across a blocked antenna. Supplement the visible cue with an accessible name or description for assistive technology. Use the existing gallery artwork for issue severity, recording, connectivity, antenna, satellites and Ethernet. Keep protocol decoding, thresholds and device-specific state descriptions in the consuming application. Red Tx and green Rx describe radio direction; select severity separately.

When adding or changing screens or controls, apply [gui-development](../gui-development/SKILL.md) for layout clarity, tooltips, and state-dependent actions.

For live measurements, distinguish zero, missing, stale and last measured values. Derive the freshness limit from the relevant data stream; unrelated traffic cannot refresh it. Keep command acceptance distinct from confirmation by fresh equipment state. On reconnect, clear pending actions and activity baselines. Verify the states relevant to the change, including loss and recovery of data, using an offscreen interaction test where practical.

Use the existing project's preferences when extending a screen. A candidate for changing the shared library or these instructions goes through user confirmation before editing or publication.
