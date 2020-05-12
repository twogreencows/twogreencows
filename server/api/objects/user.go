/* A user has multiple  greenhouses, and can access them remotely through the web.
   - on the web he must login has a session for each login session. He can have multiple logins
   - Maybe he should register all his web brwosers and iPad and iPhones to make them trustable
   -

*/

package objects

import (
	"time"
)

type user struct {
	uuid           string
	username       string
	password       string
	version        int16
	dataversion    int16
	lastUpdateDate time.Time
	session_uuid   []string
}
