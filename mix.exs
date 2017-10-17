defmodule Mix.Tasks.Compile.Libejdb do
  def run(_) do
    if match? {:win32, _}, :os.type do
      :not_implemented
    else
      System.cmd "make", ["libejdb"], stderr_to_stdout: true ,into: IO.stream(:stdio, :line)
    end
  end
end

defmodule Ejdb.Mixfile do
  use Mix.Project

  def project do
    [
      app: :ejdb,
      version: "0.1.0",
      elixir: "~> 1.5",
      compilers: [:libejdb] ++ Mix.compilers,
      deps: deps()
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    []
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:libejdb, github: "Softmotions/ejdb", tag: "v1.2.12", app: false, compile: false},
      {:nifty, github: "parapluu/nifty", branch: "master", app: false, compile: "make", manager: [:make]},
      {:rebar, path: "deps/nifty/deps/rebar", compile: false, app: false},
      {:erlydtl, path: "deps/nifty/deps/erlydtl", compile: false, app: false},
      # {:dep_from_hexpm, "~> 0.3.0"},
      # {:dep_from_git, git: "https://github.com/elixir-lang/my_dep.git", tag: "0.1.0"},
    ]
  end
end
