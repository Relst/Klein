//
// Created by Karanbir Singh on 27/11/2025.
//

#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "App.h"

namespace Klein{
    bool m_debugPanelActivated = false;

    class DebugPanel {
        public:
          DebugPanel();
          void Start();

        private:
          void Init();

    };

    void ActivateDebugPanel() {
      m_debugPanelActivated = true;
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(App::Get().GetWindow()->GetNativeWindow(), true);
      ImGui_ImplOpenGL3_Init("#version 410");
    };

    void DeactivateDebugPanel() {
      m_debugPanelActivated = false;
    };

}



#endif //DEBUGPANEL_H
