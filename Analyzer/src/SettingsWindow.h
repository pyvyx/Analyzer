#pragma once
#include <string>
#include <vector>

#include "ImGui/imgui.h"

#include "Window.h"
#include "Serial.h"

class SettingsWindow
{
private:
    std::vector<Serial::Port> m_Ports = Serial::PortListener::GetPorts();
    std::string m_PortsString;
    int m_SelectedBaudRate = 0;
    int m_SelectedPort = 0;
    bool m_CleanupGraphs = false;
public:
    explicit SettingsWindow() 
        : m_PortsString([&]() {
                std::string content;
                for (const Serial::Port& port : m_Ports)
                    content += port.com + '\n' + port.device + '\0';
                return content; 
            }()) {}

    const std::string& GetSelectedPort() const { return m_Ports[(size_t)m_SelectedPort].com; }
    int GetSelectedBaudRate() const { return m_SelectedBaudRate; }
    bool CleanupGraphs() const { return m_CleanupGraphs; }

    bool ConnectClicked(float windowWidth, bool connected)
    {
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ windowWidth, 43.f });
        ImGui::Begin("##Port selection", nullptr, IMGUI_WINDOW_FLAGS);
        const bool clicked = ImGui::Button(connected ? "Disconnect" : "Connect", {150, 0});
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        ImGui::Combo("##PortCombo", &m_SelectedPort, m_PortsString.c_str());
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        ImGui::Combo("baud", &m_SelectedBaudRate, Serial::Serial::BaudRates.data());
        ImGui::SameLine();
        ImGui::Checkbox("Cleanup Graphs", &m_CleanupGraphs);
        if(ImGui::IsItemHovered())
            ImGui::SetTooltip("Cleanup graphs by removing elements that have a similar y value.\n(Improves performance, however you may encounter bugs or the graphs might not look like what you expect)");
        ImGui::SameLine(windowWidth - 250);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        return clicked;
    }
};