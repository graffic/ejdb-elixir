defmodule Ejdb do
  @moduledoc """
  EJDB embeddable database with MongoDB like features
  """

  @on_load { :init, 0 }

  app = Mix.Project.config[:app]

  def init do
    path = :filename.join(:code.priv_dir(unquote(app)), 'ejdb')
    :ok = :erlang.load_nif(path, 0)
  end

  def version do
    raise "Missing NIF"
  end
end
