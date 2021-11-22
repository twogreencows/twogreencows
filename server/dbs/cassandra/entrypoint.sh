#!/usr/bin/env bash
## 
## This script will generate a patched docker-entrypoint.sh that:
## - executes any *.sh script found in /docker-entrypoint-initdb.d
## - boots cassandra up
## - executes any *.cql script found in docker-entrypoint-initdb.d
##
## It is compatible with any cassandra:* image
##
## Create script that executes files found in docker-entrypoint-initdb.d/

echo "1 Executing new entrypoint.sh script. create run-init script"
cat <<'EOF' >> /run-init-scripts.sh

#!/usr/bin/env bash

echo "==IN RUN-INIT"
ps aux
echo $$
echo $!
LOCK=/var/lib/cassandra/_init.done
STARTLOCK=/var/lib/cassandra/_init.start
INIT_DIR=/docker-entrypoint-initdb.d

if [ -f "$LOCK" ]; then
    echo "@@ Initialization already performed."
    exit 0
else
    echo "No Lock present"
fi

if [ -f "$STARTLOCK" ]; then
   echo "@@ Initialization already in process"
else
    echo "Creating startlock file"
    touch $STARTLOCK
    pwd
    echo "$INIT_DIR"

    if [ -d $INIT_DIR ]; then
        cd $INIT_DIR
        exit 0
        pwd
        echo "@@ Executing bash scripts found in $INIT_DIR"
        ls 
        # execute scripts found in INIT_DIR
        for f in $(find . -type f -name "*.sh" -executable -print | sort); do
            echo "$0: sourcing $f"
            . "$f"
            echo "$0: $f executed."
        done
    else
        pwd
        echo "WARNING: No docker-entrypoint-initdb.d"
        rm $STARTLOCK
        exit 0
    fi

    # wait for cassandra to be ready and execute cql in background
    (
    echo "IN SUBROUTINE"
    if [ -d $INIT_DIR ]; then
        echo "IT EXISTS"
        pwd
        which cqlsh
        cqlsh --debug -e 'describe cluster'
        while ! cqlsh -e 'describe cluster' > /dev/null 2>&1; echo "ALAL"; do sleep 6; done
        echo "$0: Cassandra cluster ready: executing cql scripts found in $INIT_DIR"
        for f in $(find . -type f -name "*.cql" -print | sort); do
            echo "$0: running $f"
            cqlsh -f "$f"
            echo "$0: $f executed"
        done

        echo "DONE"
        # mark things as initialized (in case /var/lib/cassandra was mapped to a local folder)
        touch $LOCK
    else
        echo "Problem starting up"
    fi
    ) &
    rm $STARTLOCK
fi
EOF

## Patch existing entrypoint to call our script in the background
# This has been inspired by https://www.thetopsites.net/article/51594713.shtml

echo "2 - testing existent of docker-entrypoint "
EP=/patched-entrypoint.sh
if [ -f /docker-entrypoint.sh ]; then
    echo " == docker-entrypoint.sh s existing"
else
    touch /docker-entrypoint.sh
fi

echo "3 - Doing the sed manipulation"
sed '$ d' /docker-entrypoint.sh > $EP
cat <<'EOF' >> $EP
echo "$@"
/run-init-scripts.sh &
exec "$@"
EOF

echo "4 - make both script executable"
# Make both scripts executable
chmod +x /run-init-scripts.sh
chmod +x $EP

echo "5 - call the new entrypoint"
echo $EP
cat $EP
echo $@

# Call the new entrypoint
$EP "$@"
