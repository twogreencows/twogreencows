/* A user has multiple  greenhouses, and can access them remotely through the web.
   - on the web he must login has a session for each login session. He can have multiple logins
   - Maybe he should register all his web brwosers and iPad and iPhones to make them trustable
   -

*/

package objects

import (
	"time"
)

type session struct {
	uuid         string
	version      int16
	data_version int16
	key          string
	start_date   time.Time
	end_date     time.Time
	owner_uuid   string
	device_uuid  string
}
