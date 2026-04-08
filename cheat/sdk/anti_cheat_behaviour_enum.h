#pragma once
enum AntiCheatBehaviourEnum {
    None = 0,
    SingleDamageTooHigh = 1,
    ClearedTimeTooShort = 2,
    TooFewSkillCastsPerBattle = 3,
    DamageFrequencyTooHigh = 4,
    KillEnemyDistanceTooFar = 5,
    EnemySkillCastAverageFrequencyTooLow = 6,
    PlayerCastSkillButNotReduceATB = 7,
    PlayerCastUltimateSkillButNotReduceUSP = 8,
    PlayerMoveTooFast = 9,
    PickupOrChestOpenFrequencyTooHigh = 10
};
