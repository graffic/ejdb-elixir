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

  @doc "Open as a reader"
  def jboreader, do: 1
  @doc "Open as a writer"
  def jbowriter, do: 2
  @doc "Create if db file not exists"
  def jbocreat, do: 4
  @doc "Truncate db on open."
  def jbotrunc, do: 8
  @doc "Open without locking"
  def jbonolck, do: 16
  @doc "Lock without blocking"
  def jbolcknb, do: 32
  @doc "Synchronize every transaction"
  def jbotsync, do: 64

  @doc "Ejdblib version"
  def version, do: missing_nif()

  @doc "Open database"
  def open(filename, mode)
  def open(_, _), do: missing_nif()

  defp missing_nif, do: raise "Missing NIF"
end
