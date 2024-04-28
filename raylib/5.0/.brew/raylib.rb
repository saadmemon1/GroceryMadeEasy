class Raylib < Formula
  desc "Simple and easy-to-use library to learn videogames programming"
  homepage "https://www.raylib.com/"
  url "https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz"
  sha256 "98f049b9ea2a9c40a14e4e543eeea1a7ec3090ebdcd329c4ca2cf98bc9793482"
  license "Zlib"
  head "https://github.com/raysan5/raylib.git", branch: "master"

  depends_on "cmake" => :build

  on_linux do
    depends_on "libx11"
    depends_on "libxcursor"
    depends_on "libxi"
    depends_on "libxinerama"
    depends_on "libxrandr"
    depends_on "mesa"
    depends_on "mesa-glu"
  end

  def install
    system "cmake", "-S", ".", "-B", "build",
                               "-DBUILD_SHARED_LIBS=ON",
                               "-DMACOS_FATLIB=OFF",
                               "-DBUILD_EXAMPLES=OFF",
                               "-DBUILD_GAMES=OFF",
                               *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
    system "cmake", "-S", ".", "-B", "build-static",
                               "-DBUILD_SHARED_LIBS=OFF",
                               "-DMACOS_FATLIB=OFF",
                               "-DBUILD_EXAMPLES=OFF",
                               "-DBUILD_GAMES=OFF",
                               *std_cmake_args
    system "cmake", "--build", "build-static"
    lib.install "build-static/raylib/libraylib.a"
  end

  test do
    (testpath/"test.c").write <<~EOS
      #include <stdlib.h>
      #include <raylib.h>
      int main(void)
      {
          int num = GetRandomValue(42, 1337);
          return 42 <= num && num <= 1337 ? EXIT_SUCCESS : EXIT_FAILURE;
      }
    EOS
    flags = if OS.mac?
      %w[
        -framework Cocoa
        -framework IOKit
        -framework OpenGL
      ]
    else
      %w[
        -lm
        -ldl
        -lGL
        -lpthread
      ]
    end
    system ENV.cc, "test.c", "-o", "test", "-L#{lib}", "-lraylib", *flags
    system "./test"
  end
end
