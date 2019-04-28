# mapmaker

Build pixel-perfect 2D maps for your game.

Takes a directory with images as an input and allows positioning them on a map.

Usage:
mapmaker map.json imagesDirectory/

![Screenshot](/Doc/screenshot.png?raw=true "MapMaker Screenshot")

## Hotkeys

Key                | Action
------------------ | -------------
Arrow Key          | Move item by 1 px
Alt + Drag         | Clone item
Alt + Arrow Key    | Clone item and place nearby (useful for cloning ladder parts, road pieces etc)
Delete / Backspace | Delete item
Shift + Drag       | Add items to selection
Ctrl + Click       | Add item to selection
Esc                | Deselect all

## Image metadata

For `imagename.png`, optionally create `imagename-meta.json`.

Following fields are supported:

 - `dockpoints: [ { x: Double, y, Double } ]` - dock to specified points while dragging.

Example:

For `ladder.png`, create `ladder-meta.json`:

```
{
  "dockPoints": [
    { "x": 5, "y": 0 },
    { "x": 5, "y": 14 }
  ]
}
```

