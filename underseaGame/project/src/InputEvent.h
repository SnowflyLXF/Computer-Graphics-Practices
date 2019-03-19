#pragma once

enum class InputEvent {
    // Patrick Controls
        TurboButtonPressed,
        UseHeadlight,

    // Menu controls
        ToggleHelpMenu,
        QuitMenu,

    // Camera controls
            SwitchCamera,
            
    /// Graphic controls
            ToggleCaustics,
            UseShadows,
            UseWireframe
};