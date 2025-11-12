#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <memory>
#include <string>

using namespace ftxui;

Component MakeStyledButton(const std::string &label,
                           std::function<void()> on_click);

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // Estados e valores iniciais
  bool sidebarOpen = false;

  int selectedMusic = 0;
  int selectedPlaylist = 0;
  int selectedTab = 0;
  int selectedSetting = 0;

  int initialVolumeSlidebar = 10;

  // Search bar
  std::string searchBarEntries;

  // menu simples de seleção de musicas
  std::vector<std::string> musicMenuEntries = {
      "Music 1",  "Music 2",  "Music 3",  "Music 4",  "Music 5",  "Music 6",
      "Music 7",  "Music 8",  "Music 9",  "Music 10", "Music 11", "Music 12",
      "Music 13", "Music 14", "Music 15", "Music 16", "Music 17", "Music 18",
      "Music 19", "Music 20", "Music 21", "Music 22", "Music 23", "Music 24",
      "Music 25", "Music 26", "Music 27", "Music 28", "Music 29", "Music 30",
      "Music 31", "Music 32", "Music 33", "Music 34", "Music 35", "Music 36",
      "Music 37", "Music 37",
  };

  std::vector<std::string> playlistMenuEntries = {
      "Playlist 1", "Playlist 2", "Playlist 3", "Playlist 4", "Playlist 5",

  };

  std::vector<std::string> tabLabels = {"Playlist", "Favorites",
                                        "Recently Played"};

  std::vector<std::string> settingsMenuEntries = {"Play",
                                                  "Pause",
                                                  "Stop",
                                                  "Next",
                                                  "Previous",
                                                  "Toggle Shuffle",
                                                  "Toggle Continue",
                                                  "Add music to queue",
                                                  "Remove music from queue",
                                                  "View music queue",
                                                  "Add to Playlist",
                                                  "Remove from playlist",
                                                  "Add to favorites",
                                                  "Remove from favorites",
                                                  "Add music",
                                                  "Remove music",
                                                  "Music info",
                                                  "Playlist info"};

  // Search bar
  Component searchBar = Input(&searchBarEntries, "");

  // Volume slidebar
  auto volumeSlidebar = Slider("", &initialVolumeSlidebar, 0, 50, 1);

  // Music menu
  MenuOption musicMenuOptions;
  auto musicMenu = Menu(&musicMenuEntries, &selectedMusic, musicMenuOptions);

  // Playlist menu
  MenuOption playlistMenuOptions;
  auto playlistMenu =
      Menu(&playlistMenuEntries, &selectedPlaylist, playlistMenuOptions);

  // Tabs menu
  MenuOption tabsMenuOptions = MenuOption::HorizontalAnimated();
  tabsMenuOptions.underline.color_active = Color::Red;
  auto tabsMenu = Menu(&tabLabels, &selectedTab, tabsMenuOptions);

  // Sidebar menu
  MenuOption sidebarMenuOptions;
  auto sidebarMenu =
      Menu(&settingsMenuEntries, &selectedSetting, sidebarMenuOptions);

  // Botão sidebar
  auto toggleSidebarButton =
      MakeStyledButton("Menu", [&sidebarOpen] { sidebarOpen = !sidebarOpen; });

  // Botão sair
  auto exitButton = MakeStyledButton(
      "Sair", [] { ScreenInteractive::Active()->ExitLoopClosure()(); });

  // Render topbar
  auto topBarElements = Container::Horizontal({volumeSlidebar, searchBar});

  auto topBarRender = Renderer(topBarElements, [&] {
    return hbox({
        filler(),
        text("Search:"),
        searchBar->Render() | size(WIDTH, GREATER_THAN, 90) |
            bgcolor(Color::RGB(127, 127, 127)),
        filler() | flex_grow,
        separator(),
        filler() | flex_grow,
        text("Volume:"),
        volumeSlidebar->Render() | size(WIDTH, GREATER_THAN, 50) | flex,
        filler(),
    });
  });

  // Render tabs menu
  auto tabsElements = Container::Vertical({tabsMenu, playlistMenu});

  auto tabsRender = Renderer(tabsElements, [&] {
    return vbox({
        tabsMenu->Render(),
        playlistMenu->Render() | flex,
    });
  });

  // Render conteúdo principal
  auto mainContentElements = Container::Vertical(
      {topBarRender, tabsRender, musicMenu, toggleSidebarButton, exitButton});

  auto mainContentRender = Container::Vertical({
      Renderer(mainContentElements,
               [&] {
                 return vbox({toggleSidebarButton->Render(), separator(),
                              topBarRender->Render(), separator(),

                              hbox({
                                  musicMenu->Render() | borderRounded | flex,
                                  tabsRender->Render() | borderRounded |
                                      bgcolor(Color::RGB(30, 30, 30)),
                              })});
               }),
  });

  // Render barra lateral
  auto sidebarElements = Container::Vertical({sidebarMenu});

  auto sidebarRender = Renderer(sidebarElements, [&] {
    if (!sidebarOpen) {
      return text("") | size(WIDTH, EQUAL, 0);
    }
    return vbox({text(" Menu Lateral") | bold | center, separator(),
                 sidebarMenu->Render() | flex, filler()}) |
           border | bgcolor(Color::RGB(30, 30, 30)) | size(WIDTH, EQUAL, 30);
  });

  // Guarda menu lateral e main content em container horizontal
  auto root = Container::Horizontal({sidebarRender, mainContentRender});

  // Renderiza tudo(root)
  auto renderer = Renderer(root, [&] {
    return hbox(
        {sidebarRender->Render(),
         vbox({text("Music Player") | center | bold | color(Color::Green),
               separator(), mainContentRender->Render() | flex, separator(),
               text("TAB: navegar • ENTER: clicar") | center | dim}) |
             flex | border});
  });

  screen.Loop(renderer);

  return 0;
}

// Função para botão simples
Component MakeStyledButton(const std::string &label,
                           std::function<void()> on_click) {
  // Config do botão
  ButtonOption option = ButtonOption::Animated();

  auto button = Button(" " + label + " ", std::move(on_click), option);

  // Aplica estilo visual
  return Renderer(button, [button, label] {
    return button->Render() |
           size(WIDTH, EQUAL, static_cast<int>(label.size() + 5)) | center |
           borderRounded;
  });
}
