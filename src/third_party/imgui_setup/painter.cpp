
// painter.cpp

#include "imgui_setup.h"

#define DEFAULT_CLEAR_COLOR 0x00, 0x00, 0x00, 0xFF

Painter* Painter::instance = nullptr;

Painter&
Painter::Instance()
{
    if(instance) return *instance;
    instance = new Painter();
    return *instance;
}

int
Painter::Init(const char* title, const IRect& layout)
{
    if(!init_flag) return 0;

    // Setup SDL
    init_flag = SDL_Init(SDL_INIT_EVERYTHING);
    if(init_flag != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return init_flag;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // 打开音频设备

    // From 2.0.18: Enable native IME.
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    window = SDL_CreateWindow(
        title ? title : "SDL2 ImGui",
        layout.x,
        layout.y,
        layout.w,
        layout.h,
        (SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)

    );

    if(window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        init_flag = -1;
        return init_flag;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        init_flag = -1;
        return init_flag;
    }


    // 初始化 Dear ImGui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    imgui_io = &ImGui::GetIO();
    imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    imgui_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    imgui_io->Fonts->AddFontDefault();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    return init_flag;
}

int
Painter::Quit()
{
    if(init_flag) return 1;

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio(); // 关闭音频设备

    TTF_Quit(); // 退出TTF
    Mix_Quit(); // 退出Mixer
    IMG_Quit(); // 退出IMG
    SDL_Quit(); // 退出SDL

    return 0;
}

void
Painter::On_frame_begin(EventCallback f) const
{
    static Event e;
    while(SDL_PollEvent(&e))
    {
        f(e); // 用户自定义的回调函数，用于处理用户自定义的事件
        ImGui_ImplSDL2_ProcessEvent(&e);
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
}

void
Painter::On_frame_end(Callback f) const
{
    ImGui::Render(); // Rendering

    SDL_RenderSetScale(renderer, imgui_io->DisplayFramebufferScale.x, imgui_io->DisplayFramebufferScale.y); // 设置渲染目标

    if(f)
    {
        f(); // 用户自定义的回调函数，用于渲染用户自定义的内容
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, DEFAULT_CLEAR_COLOR);
        SDL_RenderClear(renderer);
    }

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer); // 渲染imgui

    SDL_RenderPresent(renderer); // 显示渲染结果
}

float
Painter::Get_delta_time() const
{
    return imgui_io->DeltaTime;
}

float
Painter::Get_frame_rate() const
{
    return imgui_io->Framerate;
}

bool
Painter::Make_message_box(const char* title, const char* message) const
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, window);
}

void
Painter::Set_resorces_path(const char* file_path)
{
    strcpy(resources_path, file_path); // 将file_path复制到resources_path
}

Texture*
Painter::LoadTexture(const char* file_path) const
{
    char full_path[_MAX_PATH];
    sprintf(full_path, "%s/%s", resources_path, file_path);
    return IMG_LoadTexture(renderer, full_path);
}

Sound*
Painter::LoadWAV(const char* file_path) const
{
    char full_path[_MAX_PATH];
    sprintf(full_path, "%s/%s", resources_path, file_path);
    return Mix_LoadWAV(full_path);
}

Music*
Painter::LoadMUS(const char* file_path) const
{
    char full_path[_MAX_PATH];
    sprintf(full_path, "%s/%s", resources_path, file_path);
    return Mix_LoadMUS(full_path);
}

void
Painter::Create_texture(Texture*& texture, int tex_wide, int tex_high) const
{
    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        tex_wide,
        tex_high

    );

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // 设置纹理混合模式，以支持透明度
}

void
Painter::Destroy_texture(Texture*& texture) const
{
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void
Painter::Render_target(Texture* texture, const View* view)
{
    SDL_SetRenderTarget(renderer, texture);

    if(view) painter_view = view;
}

void
Painter::Render_color(const Color& color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void
Painter::Render_clear(const Color& color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void
Painter::Render_clear() const
{
    SDL_RenderClear(renderer);
}
