module {
  "name": "bs-sourcedirs",
  "description": "Produce a comma-separated string list of the directories `bsb.exe` is aware of. Run with, e.g. `jq -r 'include \"./dirs\"; dirs' ./lib/bs/.sourcedirs.json`"
};

def dirs:
  [.dirs, [.pkgs[][1]]] | flatten | join(",");