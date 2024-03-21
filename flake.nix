# Copyright © 2024 soupglasses <sofi+git@mailbox.org>
#
# Licensed under the EUPL, with extension of article 5 (compatibility
# clause) to any licence for distributing derivative works that have
# been produced by the normal use of the Work as a library.
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    systems.url = "github:nix-systems/default-linux";

    flake-utils.url = "github:numtide/flake-utils";
    flake-utils.inputs.systems.follows = "systems";
    devshell.url = "github:numtide/devshell";
    devshell.inputs.nixpkgs.follows = "nixpkgs";
    devshell.inputs.flake-utils.follows = "flake-utils";
  };
  outputs = {
    nixpkgs,
    systems,
    devshell,
    ...
  }: let
    eachSystem = f:
      nixpkgs.lib.genAttrs (import systems) (system:
        f {
          inherit system;
          pkgs = import nixpkgs { inherit system; };
        });
    project_name = "sw2-alg";
  in {
      packages = eachSystem ({pkgs, ...}: rec {
        default = sw2-alg;
        sw2-alg = pkgs.callPackage ({
          clangStdenv,
          cmake,
          fetchFromGitHub,
        }: let
          # Submodules do not get fetched by Nix Flakes when src is a path. Link our
          # dependencies manually instead.
          mtest = fetchFromGitHub {
            owner = "MortenSchou";
            repo = "mtest";
            rev = "710d38da24405761649e759532b968e6306c1f9e";
            hash = "sha256-3ruUEyezFXnoCh4wI5Rgpn0B1AS56jIglEiPViZQMSk=";
          };
        in
          clangStdenv.mkDerivation {
            name = "sw2-alg";
            src = ./.;
            nativeBuildInputs = [cmake];
            preConfigure = ''
              # Workaround: https://github.com/NixOS/nix/issues/8442
              [ -d "extern/mtest" ] && rm -r extern/mtest
              ln -s ${mtest} extern/mtest
            '';
            doCheck = true;
            env.NIX_CFLAGS_COMPILE = "-O2 -Wall -Wextra -Wpedantic";
            meta.mainProgram = project_name;
          }) {};
      });
      devShells = eachSystem ({pkgs, system}: {
        default = devshell.legacyPackages.${system}.mkShell {
          name = project_name;
          packages = with pkgs; [
            clang
            cmake
            gnumake
            pkg-config
          ];
          commands = [
            {
              name = "build";
              help = "run incremental build and test";
              command = ''
                BUILD_DIR=$PRJ_ROOT/build
                mkdir -p $BUILD_DIR
                pushd $BUILD_DIR
                [ ! -L ../compile_commands.json ] && ln -sf ./build/compile_commands.json ../compile_commands.json
                CMAKE_EXPORT_COMPILE_COMMANDS=ON cmake -S $PRJ_ROOT -B . && \
                  cmake --build . && \
                  ctest --test-dir . --output-on-failure
                popd
              '';
            }
            {
              name = "run";
              help = "build and test the project, then run the main program";
              command = "build && $PRJ_ROOT/build/src/$${1:-${nixpkgs.lib.escapeShellArg project_name}}";
            }
            {
              name = "clean";
              help = "clear up generated files from build command";
              command = ''
                [ -d $PRJ_ROOT/.cache ] && rm -rf $PRJ_ROOT/.cache
                [ -d $PRJ_ROOT/build ] && rm -rf $PRJ_ROOT/build
                [ -L $PRJ_ROOT/compile_commands.json ] && rm $PRJ_ROOT/compile_commands.json
                exit 0
              '';
            }
          ];
        };
      });
  };
}
