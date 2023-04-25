# web-helloworld-cpp
![](https://img.shields.io/github/license/open-horizon-services/web-helloworld-cpp) ![](https://img.shields.io/badge/architecture-x86-green) ![Contributors](https://img.shields.io/github/contributors/open-horizon-services/web-helloworld-cpp.svg)

## Description

Extremely simple HTTP server (written in C++) that responds on port 8000 with a hello message.

## Prerequisites 

**Docker**: Docker is needed to make and run the container. Make sure it is installed in on the Management Hub, being able to run basic Docker commands and make the container.

**Management Hub**: [Install the Open Horizon Management Hub](https://open-horizon.github.io/quick-start) or have access to an existing hub in order to publish this service and register your edge node.  You may also choose to use a downstream commercial distribution based on Open Horizon, such as IBM's Edge Application Manager.  If you'd like to use the Open Horizon community hub, you may [apply for a temporary account](https://wiki.lfedge.org/display/LE/Open+Horizon+Management+Hub+Developer+Instance) and have credentials sent to you.

**Edge Node**: You will need an x86 computer running Linux or macOS, or a Raspberry Pi computer (arm64) running Raspberry Pi OS, Ubuntu or Fedora to install and use Node-RED deployed by Open Horizon.  You will need to install the Open Horizon agent software, anax, on the edge node and register it with a hub.

## Installation

Clone the `web-helloworld-cpp` GitHub repo from a terminal prompt on the edge node and enter the folder where the artifacts were copied.

  NOTE: This assumes that `git` has been installed on the edge node.

  ``` shell
  git clone https://github.com/open-horizon-services/web-helloworld-cpp
  cd web-helloworld-cpp
  ```

Run `make clean` to confirm that the "make" utility is installed and working.

Confirm that you have the Open Horizon agent installed by using the CLI to check the version:

  ``` shell
  hzn version
  ```

  It should return values for both the CLI and the Agent (actual version numbers may vary from those shown):

  ``` text
  Horizon CLI version: 2.30.0-744
  Horizon Agent version: 2.30.0-744
  ```

  If it returns "Command not found", then the Open Horizon agent is not installed.

  If it returns a version for the CLI but not the agent, then the agent is installed but not running.  You may run it with `systemctl horizon start` on Linux or `horizon-container start` on macOS.

Check that the agent is in an unconfigured state, and that it can communicate with a hub.  If you have the `jq` utility installed, run `hzn node list | jq '.configstate.state'` and check that the value returned is "unconfigured".  If not, running `make agent-stop` or `hzn unregister -f` will put the agent in an unconfigured state.  Run `hzn node list | jq '.configuration'` and check that the JSON returned shows values for the "exchange_version" property, as well as the "exchange_api" and "mms_api" properties showing URLs.  If those do not, then the agent is not configured to communicate with a hub.  If you do not have `jq` installed, run `hzn node list` and eyeball the sections mentioned above.

NOTE: If "exchange_version" is showing an empty value, you will not be able to publish and run the service.  The only fix found to this condition thus far is to re-install the agent using these instructions:

``` shell
hzn unregister -f # to ensure that the node is unregistered
systemctl horizon stop # for Linux, or "horizon-container stop" on macOS
export HZN_ORG_ID=myorg   # or whatever you customized it to
export HZN_EXCHANGE_USER_AUTH=admin:<admin-pw>   # use the pw deploy-mgmt-hub.sh displayed
export HZN_FSS_CSSURL=http://<mgmt-hub-ip>:9443/
curl -sSL https://github.com/open-horizon/anax/releases/latest/download/agent-install.sh | bash -s -- -i anax: -k css: -c css: -p IBM/pattern-ibm.helloworld -w '*' -T 120
```

## Usage

Begin by editing the variables at the top of the Makefile as desired. If you plan to push it to a Docker registery, make sure you give your docker ID. You may also want to create unique names for your **service** and **pattern** (necessary if you are sharing a tenancy with other users and you are all publishing this service).

To play with this outside of Open Horizon:

```
make build
make run
...
make test
make stop
```

When you are ready to try it inside Open Horizon:

```
docker login
hzn key create **yourcompany** **youremail**
make build
make push
make publish-service
make publish-pattern
```

Once it is published, you can get the agent to deploy it:

```
hzn unregister -f
hzn register --pattern "${HZN_ORG_ID}/$(PATTERN_NAME)
```

Then you can watch the agreement form, see the container run, then test it:

```
watch hzn agreement list
... (runs forever, so press Ctrl-C when you want to stop)
docker ps
make test
```

Then when you are done you can get the agent to stop running it:

```
hzn unregister -f
```

## Advanced details

### All Makefile targets

* `default` - init run browse
* `build` - manually make the build of the Docker container 
* `dev` - stop current build and manually run Docker container locally
* `run` - manually run the Docker container locally
* `test` - test the local port number 8000
* `push` - push the container to a container registry
* `publish-service` - Publish the service definition file to the hub in your organization
* `publish-pattern` - Publish the pattern definition file to the hub in your organization
* `register-pattern` - Register the pattern definition file to the hub in your organization
* `stop` - stop the running container
* `clean` - remove the container from the container registry

### Authors

* [Troy Fine](https://github.com/t-fine)
* [John Walicki](https://github.com/johnwalicki)
