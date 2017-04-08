#!/bin/bash
set -x

fillsgz="${1}"
pricesgz="${2}"

gunzip -fk ${fillsgz}
gunzip -fk ${pricesgz}

fills=${"${fillsgz}"%.gz}
prices=${"${pricesgz}"%.gz}

./positionservice.exe ${fills} ${prices}
