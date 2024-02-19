# Purpose

Use this file to breakdown tasks and track progess.

Please update this file with anything you think needs done we can discuss stuff further.


# Stage One
- [x] Display Prompt  -  Display a prompt to the screen while waiting on the users input
- [x] Read users input - Saving it in an appropriately named variable 
- [x] Parse the input checking for type and character limit
- [x] Split the input into 'tokens' separated by white space (512)
- [x] Allow continues reading until exit triggered through input "exit" and ctr-D


# Stage Two
- [x] Use fork() to create a child process
- [x] Prompt the parent process to wiat on the child to complete
- [x] Have the child process run the exec() function to run external commands  using the previously entered tokens.
- [x] catch errors appropriatly (moday 29th lecture)


# Stage Two Strech
- [x] Write test file
- [x] Add final perrors
- [x] Possible error file


# Stage Three
- [x] get the home and current directory and save them for use on shutdown
- [x] save the PATH at the start of the program
- [x] have the program start in the users home dir
- [x] use getenv() to get users HOME dir
- [x] change the current dirrectory of the program using chdir()
- [x] use getcwd() finction to check this is working
- [x] implement getpath() setpath() functions
- [x] handle inproper param entered to getpath() and setpath() note: these functions return int values dependent on success


# Stage Four
- [x] check for cd command
- [x] check for to many args
- [x] change to an existing directory
- [x] show error for if a dir does not exist

# Stage Four - FIX
- [x] cd into a file should show an error saying its a file
- [x] getpath setpath gethome sethome
- [x] cd no arguments 
- [x] not enough arguments for change path and change home# Stage Five


# Stage Five
- [x] Create a list of Strings to hold the commands entered during runtime
- [x] Each time a command is ran add it to the History array
- [x] Check for '!' being the first character
- [ ] Invoke command from history
        - [x] !! ==> print last command
        - [x] !1 ==> print first command
        - [x] !-1 ==> print first command 
- [x] If the number is greater or lower than the length of the History array throw error
- [x] Treat 0 of the array as 1 and run the command at the index entered 
        - [x] '!-1' ==> run the 0th command
        - [x] '!-4' ==> run the 3rd command
- [ ] Pick a method of handling a full array
        - circular array?
        - shift all elements to the left 1-2-3 becomes 0-1-2?
        - reallocate array to increase size?
- [x] Print History if the form of \<No> \<command>
- [x] If command found in history it should be ran like any other command but there should be no history check 

# Stage Six
- [ ] Create a new file to save the history to
        - might need to save positions if doing circular array.
- [ ] Save a history command per line of the txt file to make it easier to read back in
- [ ] Read history from the file
- [ ] Save history to the history array with all the corrct pointers
- [ ] Make sure the history stays in the same order between instances

# Stage Seven

- [x] look into 2 dimensional arrays for storing alias and the original command
- [x] or set up another structure to map keys to \(values alias to command)
- [x] one->many   alias->command
- [x] check for alias first (takes priority over other commands)
- [x] only one alias check 
- [x] alias <name> <command>
- [ ] unalias <command>
- [x] <alias> [runs alias]
- [x] "alias" [prints all alias]
- [ ] if alias exists jsut replace it with the new one
- [ ] extend alias array 




## Getting started

To make it easy for you to get started with GitLab, here's a list of recommended next steps.

Already a pro? Just edit this README.md and make it your own. Want to make it easy? [Use the template at the bottom](#editing-this-readme)!

## Add your files

- [ ] [Create](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#create-a-file) or [upload](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#upload-a-file) files
- [ ] [Add files using the command line](https://docs.gitlab.com/ee/gitlab-basics/add-file.html#add-a-file-using-the-command-line) or push an existing Git repository with the following command:

```
cd existing_repo
git remote add origin https://gitlab.cis.strath.ac.uk/gvb22143/cs-210-simple-shell.git
git branch -M main
git push -uf origin main
```

## Integrate with your tools

- [ ] [Set up project integrations](https://gitlab.cis.strath.ac.uk/gvb22143/cs-210-simple-shell/-/settings/integrations)

## Collaborate with your team

- [ ] [Invite team members and collaborators](https://docs.gitlab.com/ee/user/project/members/)
- [ ] [Create a new merge request](https://docs.gitlab.com/ee/user/project/merge_requests/creating_merge_requests.html)
- [ ] [Automatically close issues from merge requests](https://docs.gitlab.com/ee/user/project/issues/managing_issues.html#closing-issues-automatically)
- [ ] [Enable merge request approvals](https://docs.gitlab.com/ee/user/project/merge_requests/approvals/)
- [ ] [Set auto-merge](https://docs.gitlab.com/ee/user/project/merge_requests/merge_when_pipeline_succeeds.html)

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing (SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)
- [ ] [Deploy to Kubernetes, Amazon EC2, or Amazon ECS using Auto Deploy](https://docs.gitlab.com/ee/topics/autodevops/requirements.html)
- [ ] [Use pull-based deployments for improved Kubernetes management](https://docs.gitlab.com/ee/user/clusters/agent/)
- [ ] [Set up protected environments](https://docs.gitlab.com/ee/ci/environments/protected_environments.html)

***

# Editing this README

When you're ready to make this README your own, just edit this file and use the handy template below (or feel free to structure it however you want - this is just a starting point!). Thanks to [makeareadme.com](https://www.makeareadme.com/) for this template.

## Suggestions for a good README

Every project is different, so consider which of these sections apply to yours. The sections used in the template are suggestions for most open source projects. Also keep in mind that while a README can be too long and detailed, too long is better than too short. If you think your README is too long, consider utilizing another form of documentation rather than cutting out information.

## Name
Choose a self-explaining name for your project.

## Description
Let people know what your project can do specifically. Provide context and add a link to any reference visitors might be unfamiliar with. A list of Features or a Background subsection can also be added here. If there are alternatives to your project, this is a good place to list differentiating factors.

## Badges
On some READMEs, you may see small images that convey metadata, such as whether or not all the tests are passing for the project. You can use Shields to add some to your README. Many services also have instructions for adding a badge.

## Visuals
Depending on what you are making, it can be a good idea to include screenshots or even a video (you'll frequently see GIFs rather than actual videos). Tools like ttygif can help, but check out Asciinema for a more sophisticated method.

## Installation
Within a particular ecosystem, there may be a common way of installing things, such as using Yarn, NuGet, or Homebrew. However, consider the possibility that whoever is reading your README is a novice and would like more guidance. Listing specific steps helps remove ambiguity and gets people to using your project as quickly as possible. If it only runs in a specific context like a particular programming language version or operating system or has dependencies that have to be installed manually, also add a Requirements subsection.

## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Support
Tell people where they can go to for help. It can be any combination of an issue tracker, a chat room, an email address, etc.

## Roadmap
If you have ideas for releases in the future, it is a good idea to list them in the README.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
For open source projects, say how it is licensed.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.
