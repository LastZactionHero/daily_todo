require 'json'
require 'rest-client'
require './asana_downloader'
require './workspace'
require 'pry'

SELECTED_WORKSPACES = [
  'Family',
  'Personal Projects'
]
IGNORED_PROJECTS = [
  'Thanksgiving',
  'Christmas',
  'Meghan\'s Todo',
  'Aimless Browsing List'
]
asana_token = ENV['ASANA_DAILY_TODO_TOKEN']

# Workspaces
# Projects
# Tasks
#
host = "https://app.asana.com/api/1.0"

puts "Fetching Asana Tasks..."

# Fetch Workspaces
workspaces_index_url = "#{host}/workspaces"
response = RestClient.get(workspaces_index_url, { Authorization: "Bearer #{asana_token}" })
workspaces = JSON.parse(response.body)['data'].map { |d| OpenStruct.new(d) }
workspaces = workspaces.select { |ws| SELECTED_WORKSPACES.include?(ws.name) }

workspaces.each do |workspace|
  project_index_url = "#{host}/projects?workspace=#{workspace.id}"
  response = RestClient.get(project_index_url, { Authorization: "Bearer #{asana_token}" })
  workspace.projects = JSON.parse(response.body)['data'].map { |d| OpenStruct.new(d) }
  workspace.projects.reject!{ |p| IGNORED_PROJECTS.include?(p.name) }

  workspace.projects.each do |project|
    puts project.name
    tasks_index_url = "#{host}/projects/#{project.id}/tasks"
    response = RestClient.get(tasks_index_url, { Authorization: "Bearer #{asana_token}" })
    project.tasks = JSON.parse(response.body)['data'].map { |d| OpenStruct.new(d) }

    project.tasks.each_with_index do |task, idx|
      puts task.name
      task_url = "#{host}/tasks/#{task.id}"
      response = RestClient.get(task_url, { Authorization: "Bearer #{asana_token}" })
      project.tasks[idx] = OpenStruct.new(JSON.parse(response.body)['data'])
    end
  end
end

workspaces.each do |workspace|
  next if workspace.projects.flat_map{|p| p.tasks}.length == 0
  puts workspace.name
  workspace.projects.each do |project|
    tasks = project.tasks.reject{|t| t.completed}
    next if tasks.length == 0
    puts " #{project.name}"
    tasks.each do |task|
      task_str = " - #{task.name}"
      task_str += " (#{task.due_on})" if task.due_on
      puts task_str
    end
  end
  puts ""
end; 0
