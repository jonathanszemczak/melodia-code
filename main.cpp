#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

using namespace ftxui;

// === FUNÇÃO REUTILIZÁVEL: Botão com hover animado + estilo clean ===
Component MakeStyledButton(const std::string &label,
                           std::function<void()> on_click) {
  // Configuração do Button com animação de cores
  ButtonOption option = ButtonOption::Animated();

  auto btn = Button(" " + label + " ", std::move(on_click), option);

  // Aplica estilo visual: borda arredondada, tamanho fixo, fundo escuro ao
  // redor
  return Renderer(btn, [btn, label] {
    return btn->Render() |
           size(WIDTH, EQUAL, static_cast<int>(label.size() + 4)) | center |
           borderRounded | bgcolor(Color::GrayDark);
  });
}

int main() {
  const std::string middle = "middle";
  const std::string right = "right";

  // Estado da sidebar
  bool sidebar_open = false;

  // === BOTÃO: Abre/fecha sidebar ===
  auto toggle_sidebar_btn = MakeStyledButton(
      "Menu", [&sidebar_open] { sidebar_open = !sidebar_open; });

  // === BOTÃO: Sair ===
  auto exit_btn = MakeStyledButton(
      "Sair", [] { ScreenInteractive::Active()->ExitLoopClosure()(); });

  // === SIDEBAR ===
  auto sidebar = Renderer([&] {
    if (!sidebar_open) {
      return text("") | size(WIDTH, EQUAL, 0);
    }
    return vbox({text(" Menu Lateral") | bold, separator(), text(" Play"),
                 text(" Pause"), text(" Stop"), filler(),
                 text(" Fechar") | dim}) |
           border | size(WIDTH, EQUAL, 20) | bgcolor(Color::RGB(30, 30, 30));
  });

  // === CONTEÚDO PRINCIPAL ===
  auto main_content = Container::Vertical({Renderer([&] {
                                             return hbox({
                                                 text(middle) | border | flex,
                                                 text(right) | border | flex,
                                             });
                                           }),
                                           toggle_sidebar_btn, exit_btn});

  // === LAYOUT FINAL ===
  auto root = Container::Horizontal({sidebar, main_content});

  auto renderer = Renderer(root, [&] {
    return hbox(
        {sidebar->Render(),
         vbox({text("Music Player") | center | bold | color(Color::Green),
               separator(), main_content->Render(), separator(),
               text("TAB: navegar • ENTER: clicar") | center | dim}) |
             flex | border});
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);

  return 0;
}
