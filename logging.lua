--- Example of LEGE's logging facilities via the "lege.log" module
-- Logging can be useful when debugging your application, or to provide the user with information they can use when
-- filing a bug report.

local log = require "lege.log"

log.info("Log priority is currently set to " .. log.get_priority())
log.verbose("So you won't see this")
log.set_priority("verbose")
log.info("Log priority is now set to " .. log.get_priority())

log.verbose("The game's still running")
log.debug("Starting frobnication process ...")
log.info("Logged in as xx_DankMemer69_xx")
log.warn("That username is stupid")
log.error("The server rejected your username, as it is too stupid")
log.critical("Cannot continue with a user this stupid")

local msg_type = "info"
log.message(msg_type, "Yes, we're talking about *you*")
