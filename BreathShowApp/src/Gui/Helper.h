//
//  Helper.hpp
//

#pragma once

class ofApp;

namespace gui{

class Helper{

public:
    static void menu(const shared_ptr<ofApp> & app);
    static void dock(const shared_ptr<ofApp> & app);
    static ofRectangle calcCameraControlArea();
    
    
    /// Drags
    inline static void DragInt( ofParameter<int> & p, float step=1){
        int min = p.getMin();
        int max = p.getMax();
        ImGui::DragScalar(p.getName().c_str(), ImGuiDataType_S32, (int *) &p.get(), step, &min, &max);
    }
    
    inline static void DragFloat( ofParameter<float> & p, float step=0.01f){
        float min = p.getMin();
        float max = p.getMax();
        ImGui::DragScalar(p.getName().c_str(), ImGuiDataType_Float, (float *) &p.get(), step, &min, &max);
    }
    
    inline static void DragFloat3( const char * name, ofParameter<glm::vec3> & p, float step=0.01f){
        float min = p.getMin().x;
        float max = p.getMax().x;
        ImGui::DragFloat3(name, (float *) &p.get().x, step, min, max);
    }
    
    inline static void DragDouble( ofParameter<double> & p, float step=0.01f){
        double min = p.getMin();
        double max = p.getMax();
        ImGui::DragScalar(p.getName().c_str(), ImGuiDataType_Double, (double *) &p.get(), step, &min, &max, "%.2f");
    }
    
    inline static void Combo( ofParameter<int> & p, const vector<string> & items){
        //ImGui::Combo(p.getName().c_str(), (int*&)p.get(), (char*)&list[0]);
        // Using the generic BeginCombo() API, you have full control over how to display the combo contents.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        // stored in the object itself, etc.)
        const string combo_preview_value = items[p];
        ImGuiComboFlags_ flags = ImGuiComboFlags_None;
        /// ImGuiComboFlags_PopupAlignLeft
        /// ImGuiComboFlags_HeightSmall
        /// ImGuiComboFlags_HeightRegular
        /// ImGuiComboFlags_HeightLarge
        /// ImGuiComboFlags_HeightLargest
        /// ImGuiComboFlags_NoArrowButton
        /// ImGuiComboFlags_NoPreview
        /// ImGuiComboFlags_HeightMask_
        if (ImGui::BeginCombo(p.getName().c_str(), combo_preview_value.c_str(), flags)){
            for (int n = 0; n < items.size(); n++){
                const bool is_selected = (p == n);
                if (ImGui::Selectable(items[n].c_str(), is_selected))
                    p = n;
                
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    
    inline static void drawMsgLogger(const std::deque<string> & queue){
        if(queue.size() !=0 ){
            for(const string & s : queue){
                ImGui::Text("%s", s.c_str());
            }
        }
    }
};

};
