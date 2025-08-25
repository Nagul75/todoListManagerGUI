#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "todoListManager.h"


int main() {
    // 1. Initialize GLFW (the windowing library)
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "My ImGui App", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // 2. Initialize GLAD (the OpenGL loader)
    // gladLoadGL(); // Or whatever the GLAD setup function is

    // 3. Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // 4. Initialize the ImGui Backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initialize task manager
    tdl::TodoListManager manager{};

    // char buffer for gui input
    char taskNameBuffer[128] = "";
    char taskDescBuffer[256] = "";

    float save_notification_timer = 0.0f;

    // --- Main application loop ---
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Get the main viewport's size and position
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(main_viewport->WorkPos);
        ImGui::SetNextWindowSize(main_viewport->WorkSize);

        // Set special flags for a "root" window
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        window_flags |= ImGuiWindowFlags_NoNavFocus;

        // UI code
        ImGui::Begin("To-Do List Manager", nullptr, window_flags);

        // Button to trigger the popup
        if (ImGui::Button("Add New Task...")) {
            ImGui::OpenPopup("AddTaskPopup");
        }

        // make save button appear at the end of the screen simulating css:justify-between
        // Done by subtracting window width and the button width
        const float saveButtonWidth = ImGui::CalcTextSize("Save Tasks").x + ImGui::GetStyle().FramePadding.x * 2.0f;
        ImGui::SameLine(ImGui::GetWindowWidth() - saveButtonWidth - 20.0f);

        if (ImGui::Button("Save Tasks")) {
            manager.writeToFile();
            save_notification_timer = 3.0f;
        }

        ImGui::Separator();

        const std::size_t taskSize {manager.getTasksSize()};

        for (std::size_t index{}; index < taskSize; ++index)
        {
            tdl::Task& task{manager.viewTask(index)};

            ImGui::PushID(static_cast<int>(index));
            std::string checkbox_id {"##" + std::to_string(index)};
            ImGui::Checkbox(checkbox_id.c_str(), &task.m_isCompleted);
            ImGui::SameLine();
            ImGui::Text("%s", task.m_taskName.c_str());

            const float removeButtonWidth = ImGui::CalcTextSize("Remove Task").x + ImGui::GetStyle().FramePadding.x * 2.0f;
            ImGui::SameLine(ImGui::GetWindowWidth() - removeButtonWidth - 20.0f);

            if (ImGui::Button("Remove task"))
            {
                manager.removeTask(index);
                ImGui::PopID();
                break;
            }

            if (!task.m_taskDescription.empty())
            {
                ImGui::TextWrapped("%s", task.m_taskDescription.c_str());
            }

            ImGui::PopID();
            ImGui::Separator();
        }
        ImGui::SetNextWindowSize(ImVec2(1000, 600), ImGuiCond_FirstUseEver);
        if (ImGui::BeginPopupModal("AddTaskPopup"))
        {
            ImGui::Text("Task Name");
            ImGui::InputText("##TaskName", taskNameBuffer, IM_ARRAYSIZE(taskNameBuffer));

            // Makes sure that the buttons appear at the bottom of the popup
            const float bottom_margin = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::Text("Description");
            ImGui::InputTextMultiline("##Description", taskDescBuffer, IM_ARRAYSIZE(taskDescBuffer), ImVec2(0, -bottom_margin));

            if (ImGui::Button("Save"))
            {
                if (taskNameBuffer[0] != '\0')
                {
                    tdl::Task newTask{false, taskNameBuffer, taskDescBuffer};
                    manager.addTask(newTask);

                    // Clear buffers for next time
                    taskNameBuffer[0] = '\0';
                    taskDescBuffer[0] = '\0';

                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                // Clear buffers just in case
                taskNameBuffer[0] = '\0';
                taskDescBuffer[0] = '\0';
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        // Simulate toast functionality
        if (save_notification_timer > 0.0f)
        {
            constexpr float PADDING = 10.0f; // Padding from the edge of the screen

            // Calculate the position based on the display size
            ImVec2 window_pos = ImVec2(io.DisplaySize.x - PADDING, io.DisplaySize.y - PADDING);

            // Set the pivot point to the bottom-right of the notification window
            ImVec2 window_pos_pivot = ImVec2(1.0f, 1.0f);

            // Set the position of the next window to appear
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

            ImGui::Begin("Save Notification", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Saved successfully!");
            ImGui::End();

            save_notification_timer -= ImGui::GetIO().DeltaTime;
        }

        ImGui::End();

        // --- End of UI code ---

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}