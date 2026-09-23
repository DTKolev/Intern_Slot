#pragma once

#include "GameObjects/CommonManager.hpp"
#include "GameObjects/InputManager.hpp"
#include "Singleton_Visualizer.hpp"

class MasterResults : public single::GameState {

    protected:
    CommonManager& common_manager = CommonManager::GetInstance();
    InputManager& input_manager = InputManager::GetInstance();

    single::Color line_color;
    int winning_lines_amount;
    int display_line_id;
    double line_display_timer;

    single::Color frame_color;
    bool show_frames;
    double frame_display_timer;

    single::Text win;
    int win_amount;
    int display_win;
    double win_display_timer;

    single::Text bet;

    bool scatters_found;

    /**
     * @brief Calculates the x-coordinate of the center of a cell
     * based on the cell ID
     */
    auto CalculateX(int cell_id, const GridData& grid_data, bool reverse_lines) const -> float;
    /**
     * @brief Calculates the y-coordinate of the center of a cell
     * based on the cell ID
     */
    auto CalculateY(int cell_id, const GridData& grid_data, bool reverse_lines) const -> float;

    /**
     * @brief Displays a payline on top of the grid
     *
     * @param ln the payline to be drawn
     */
    void DrawLine(const Line& ln, const single::Color& color) const;
    /**
     * @brief Draws a colorful frame around a specified cell
     */
    void DrawCellFrame(const Cell& cell, const single::Color& color) const;

    /**
     * @brief Creates a log entry after each reeling and writes it to a log file;
     * Maintains a record of the last 10 reelings
     *
     * @param eng provides the absolute path to the binary
     */
    void LogGameResults(const single::Engine& eng) const;


    public:
    void OnEntry(const single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
    void OnExit() override;
};
