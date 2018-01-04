require 'rest-client'

class AsanaDownloader
  TOKEN = ENV['ASANA_DAILY_TODO_TOKEN']
  HOST = "https://app.asana.com/api/1.0"

  def initialize(asana_object)
    @asana_object = asana_object
  end

  def fetch
    url = "#{HOST}#{@asana_object.path}/projects"
    response = RestClient.get(url, { Authorization: "Bearer #{TOKEN}" })
    puts response.body
    #workspaces = JSON.parse(response.body)['data'].map { |d| Workspace.new(d['id'], d['name']) }
    #workspaces = workspaces.select { |ws| SELECTED_WORKSPACES.include?(ws.name) }
  end
end
