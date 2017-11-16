defmodule Mix.Tasks.Compile.EjdbNif do
  def run(_) do
    if match? {:win32, _}, :os.type do
      :not_implemented
    else
      {_, 0} = System.cmd(
        "make",
        ["COVERAGE=#{Mix.env == :test}", "priv/ejdb.so"],
        stderr_to_stdout: true,
        into: IO.stream(:stdio, :line)
      )
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
      test_coverage: [tool: ExCoveralls],
      compilers: [:ejdb_nif] ++ Mix.compilers,
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
      {
        :libejdb,
        github: "Softmotions/ejdb",
        tag: "v1.2.12",
        app: false,
        compile: "make -C ../.. libejdb"
      },
      {:excoveralls, "~> 0.7", only: :test},
      {:bson, "~> 0.4.4", only: :test}
    ]
  end
end
