/* A greenhouse has lots and devices. A lot has a plant type a start date, a position  and can be observed

 */

package objects

import (
	"time"
)

type greenhouse struct {
	uuid           string
	dataversion    int16
	lastUpdateDate time.Time
	owner_uuid     string
}
