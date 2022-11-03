#include "../include/log.h"

Log::Log(char* path) : auto_scroll(true), write_to_file(true), log_file_path(path) {
  Clear();
  log_file = fopen(log_file_path, "ab+");
  if (!log_file) {
    write_to_file = false;
    this->AddLog("[ERROR] Failed to open log file\n");
  }
  else {
    // TODO add some kind of session idendifier here
    fprintf(log_file, "----------------------------------------\n");
  }
}

Log::~Log() {
  if (log_file) fclose(log_file);
}

void Log::Clear() {
  buf.clear();
  line_offsets.clear();
  line_offsets.push_back(0);
}

void Log::AddLog(const char* fmt, ...) {
  ++count;
  int old_size = buf.size();
  va_list args;
  va_start(args, fmt);
  buf.appendfv(fmt, args);
  va_end(args);
  for (int new_size = buf.size(); old_size < new_size; old_size++)
    if (buf[old_size] == '\n')
      line_offsets.push_back(old_size + 1);

  if (write_to_file) {
    if (log_file) {
      int l = line_offsets[line_offsets.size() - 2];
      int r = line_offsets.back();
      fwrite(buf.begin() + l, 1, r - l, log_file);
    }
  }
}

void Log::Draw(const char* title, bool* p_open = NULL) {
  if (!ImGui::Begin(title, p_open)) {
    ImGui::End();
    return;
  }

  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &auto_scroll);
    ImGui::Checkbox("Write To File", &write_to_file);
    ImGui::EndPopup();
  }

  if (ImGui::Button("Options"))
    ImGui::OpenPopup("Options");
  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");
  ImGui::SameLine();
  filter.Draw("filter [\'-\' exclude]", -250.0f);
  ImGui::SameLine();
  bool clear = ImGui::Button("Clear");
  ImGui::Separator();
  ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

  if (clear)
    Clear();
  if (copy)
    ImGui::LogToClipboard();

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  const char* buffer = buf.begin();
  const char* buffer_end = buf.end();
  if (filter.IsActive()) {
    // In this example we don't use the clipper when filter is enabled.
    // This is because we don't have a random access on the result on our filter.
    // A real application processing logs with ten of thousands of entries may want to store the result of
    // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
    for (int line_no = 0; line_no < line_offsets.Size; line_no++) {
      const char* line_start = buffer + line_offsets[line_no];
      const char* line_end = (line_no + 1 < line_offsets.Size) ? (buffer + line_offsets[line_no + 1] - 1) : buffer_end;
      if (filter.PassFilter(line_start, line_end))
        ImGui::TextUnformatted(line_start, line_end);
    }
  }
  else {
    // The simplest and easy way to display the entire buffer:
    //   ImGui::TextUnformatted(buf_begin, buf_end);
    // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
    // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
    // within the visible area.
    // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
    // on your side is recommended. Using ImGuiListClipper requires
    // - A) random access into your data
    // - B) items all being the  same height,
    // both of which we can handle since we an array pointing to the beginning of each line of text.
    // When using the filter (in the block of code above) we don't have random access into the data to display
    // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
    // it possible (and would be recommended if you want to search through tens of thousands of entries).
    ImGuiListClipper clipper;
    clipper.Begin(line_offsets.Size);
    while (clipper.Step()) {
      for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
        const char* line_start = buffer + line_offsets[line_no];
        const char* line_end = (line_no + 1 < line_offsets.Size) ? (buffer + line_offsets[line_no + 1] - 1) : buffer_end;
        ImGui::TextUnformatted(line_start, line_end);
      }
    }
    clipper.End();
  }
  ImGui::PopStyleVar();

  if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();
  ImGui::End();
}
